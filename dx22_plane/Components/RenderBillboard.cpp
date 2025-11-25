#include "RenderBillboard.h"
#include "DirectXRender.h"
#include "Transform.h"
#include "GameObjectManager.h"
#include "Camera.h"

using namespace DirectX::SimpleMath;

RenderBillboardComponent::RenderBillboardComponent(GameObject& obj) : RenderComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER"); // ソート番号を設定
	m_Shader = std::make_unique<Shader>();
	//m_Texture = std::make_unique<Texture>();
}

void RenderBillboardComponent::Update()
{
	const auto transform = m_Object->GetComponent<TransformComponent>();
	const auto camera = GameObjectManager::GameObjectFindName("camera");

	if (transform != nullptr && camera != nullptr && m_Mesh != nullptr) {
		//定数バッファを更新
		ConstBuffer cb;

		const auto pos = transform->GetPosition();
		const auto scale = transform->GetScale();
		const auto cameraView3D = camera->GetComponent<Camera>()->GetView3D();

		Matrix rotationOnly = cameraView3D;

		// 万が一カメラのサイズが変わってしまった場合の保険の処理
		// ３×３行列として計算することで、位置情報を含まない ＝ 純粋に、回転を正規化してベクトルのみを取り出すことができる
		for (int i = 0; i < 3; ++i) {
			Vector3 axis(cameraView3D.m[0][i], cameraView3D.m[1][i], cameraView3D.m[2][i]);
			axis.Normalize();

			// 各軸を正規化して上書き
			rotationOnly.m[0][i] = axis.x;
			rotationOnly.m[1][i] = axis.y;
			rotationOnly.m[2][i] = axis.z;
		}

		rotationOnly._41 = 0.0f; // ビルボードの位置をカメラの位置に合わせる
		rotationOnly._42 = 0.0f;
		rotationOnly._43 = 0.0f;

		const Matrix t = Matrix::CreateTranslation(pos.x, pos.y, pos.z);
		const Matrix s = Matrix::CreateScale(scale.x, scale.y, scale.z);

		rotationOnly = rotationOnly.Transpose();	// 転置行列にすることで逆行列にするより処理が軽い

		cb.matrixWorld = (s * rotationOnly * t).Transpose();

		cb.color = Vector4(m_Color);

		ID3D11DeviceContext* deviceContext = DirectXRender::GetDeviceContext();

		// 描画の処理
		// トポロジーをセット（プリミティブタイプ）
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 頂点の結び方の規則
		m_Shader->SetGPU();
		m_VertexBuffer.SetGPU();
		m_IndexBuffer.SetGPU();

		Texture texture = GetTexture();
		Vector4 uvs = texture.GetUVSets();

		uvs.x = uvs.x - 1;
		uvs.y = uvs.y - 1;
		uvs.z = 1 / uvs.z;
		uvs.w = 1 / uvs.w;

		cb.matrixTex = texture.MakeUV(uvs.x, uvs.y, uvs.z, uvs.w);

		cb.inverse = m_InversionFlag;

		// 行列をシェーダーに渡す
		deviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

		const std::vector<SUBSET> subsets = m_Mesh->GetSubsets();

		const std::vector<MATERIAL> materials = m_Mesh->GetMaterials();

		std::vector<Texture> textures = m_Mesh->GetTextures();

		//マテリアル数分ループ 
		for (unsigned int i = 0; i < subsets.size(); i++)
		{
			// ここ使う
			const MATERIAL material = materials[subsets[i].MaterialIdx];

			deviceContext->UpdateSubresource(m_MaterialBuffer, 0, NULL, &material, 0, 0);

			textures[subsets[i].MaterialIdx].SetGPU();

			deviceContext->DrawIndexed(
				subsets[i].IndexNum,		// 描画するインデックス数
				subsets[i].IndexBase,		// 最初のインデックスバッファの位置	
				subsets[i].VertexBase);	// 頂点バッファの最初から使用
		}
	}
}