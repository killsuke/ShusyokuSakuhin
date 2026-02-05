#include "RenderBillboard.h"
#include "System/DirectXRender.h"
#include "Transform.h"
#include "Manager/GameObjectManager.h"
#include "Camera.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

RenderBillboardComponent::RenderBillboardComponent(GameObject& obj) : RenderComponent(obj) {
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER"); // ソート番号を設定
	m_Shader = std::make_unique<Shader>();
	//m_Texture = std::make_unique<Texture>();
}

void RenderBillboardComponent::Update()
{
	const TransformComponent* transform = m_Object->GetComponent<TransformComponent>();
	GameObject* camera = GameObjectManager::GameObjectFindName("camera");

	if (transform != nullptr && camera != nullptr && m_Mesh != nullptr) {
		//定数バッファを更新
		ConstBuffer cb;

		const Vector3 pos = transform->GetPosition();
		const Vector3 scale = transform->GetScale();
		Camera* cameraComp = camera->GetComponent<Camera>();
		const XMMATRIX cameraView3D = cameraComp->GetView3D();

		XMMATRIX rotationOnly = cameraView3D;

		// 万が一カメラのサイズが変わってしまった場合の保険の処理
		// ３×３行列として計算することで、位置情報を含まない ＝ 純粋に、回転を正規化してベクトルのみを取り出すことができる
		for (int i = 0; i < 3; ++i) {
			XMVECTOR axis = XMVectorSet(
				cameraView3D.r[0].m128_f32[i],
				cameraView3D.r[1].m128_f32[i],
				cameraView3D.r[2].m128_f32[i],
				0.0f);
			axis = XMVector3Normalize(axis);

			// 各軸を正規化して上書き
			rotationOnly.r[0].m128_f32[i] = XMVectorGetX(axis);
			rotationOnly.r[1].m128_f32[i] = XMVectorGetY(axis);
			rotationOnly.r[2].m128_f32[i] = XMVectorGetZ(axis);
		}

		// ビルボードの位置をカメラの位置に合わせる
		rotationOnly.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		const XMMATRIX t = XMMatrixTranslation(pos.x, pos.y, pos.z);
		const XMMATRIX s = XMMatrixScaling(scale.x, scale.y, scale.z);

		rotationOnly = XMMatrixTranspose(rotationOnly);	// 転置行列にすることで逆行列にするより処理が軽い

		cb.matrixWorld = XMMatrixTranspose(s * rotationOnly * t);

		cb.color = m_Color;

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

		cb.inverse = m_Inversion == RightLeft::RIGHT ? true : false;

		ID3D11Buffer* bufferDraw = DirectXRender::GetDefaultDrawBuffer();

		// 行列をシェーダーに渡す
		deviceContext->UpdateSubresource(bufferDraw, 0, NULL, &cb, 0, 0);

		const std::vector<SUBSET> subsets = m_Mesh->GetSubsets();

		const std::vector<MATERIAL> materials = m_Mesh->GetMaterials();

		std::vector<Texture> textures = m_Mesh->GetTextures();

		ID3D11Buffer* bufferMaterial = DirectXRender::GetMaterialBuffer();

		//マテリアル数分ループ 
		for (unsigned int i = 0; i < subsets.size(); i++)
		{
			// ここ使う
			const MATERIAL material = materials[subsets[i].MaterialIdx];

			deviceContext->UpdateSubresource(bufferMaterial, 0, NULL, &material, 0, 0);

			textures[subsets[i].MaterialIdx].SetGPU();

			deviceContext->DrawIndexed(
				subsets[i].IndexNum,		// 描画するインデックス数
				subsets[i].IndexBase,		// 最初のインデックスバッファの位置	
				subsets[i].VertexBase);	// 頂点バッファの最初から使用
		}
	}
}