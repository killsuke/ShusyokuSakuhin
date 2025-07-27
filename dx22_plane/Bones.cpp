#include "Bones.h"
#include "Transform.h"
#include "GameObjectManager.h"
#include "Camera.h"
using namespace DirectX::SimpleMath;

// ボーンのコンストラクタ
Bones::Bones(GameObject& obj) : RenderComponent(obj)
{
	m_sortNum = RENDER_ONE_SKIN_ANIMATION;
	m_Shader = std::make_unique<Shader>();
	m_Texture = std::make_unique<Texture>();

	m_bones = new Bone[BONE_NUM];	// ボーンの数だけメモリを確保

	// 親子関係を構築します
	m_bones[0].firstChild = &m_bones[1];	// 腰から胸
	m_bones[1].firstChild = &m_bones[2];	// 胸から頭
	m_bones[3].firstChild = &m_bones[4];	// 左肩から左腕
	m_bones[4].firstChild = &m_bones[5];	// 左腕から左手
	m_bones[6].firstChild = &m_bones[7];	// 右肩から右腕
	m_bones[7].firstChild = &m_bones[8];	// 右腕から右手
	m_bones[9].firstChild = &m_bones[10];	// 左足付け根から左膝
	m_bones[10].firstChild = &m_bones[11];	// 左膝から左足首
	m_bones[12].firstChild = &m_bones[13];	// 右足付け根から右膝
	m_bones[13].firstChild = &m_bones[14];	// 右膝から右足首

	m_bones[1].sibling = &m_bones[9];		// 腰の兄弟に左足付け根
	m_bones[9].sibling = &m_bones[12];		// 左足付け根の兄弟に右足付け根
	m_bones[2].sibling = &m_bones[3];	// 頭の兄弟に左肩
	m_bones[3].sibling = &m_bones[6];	// 左肩の兄弟に右肩



	g_combMtx = new DirectX::SimpleMath::Matrix[BONE_NUM];	// 合成変換行列の配列を確保
}

Bones::~Bones()
{
	delete[] m_bones;	// ボーンのメモリを解放
	m_bones = nullptr;	// ポインタをnullptrに設定

	delete[] g_combMtx;	// 合成変換行列のメモリを解放
	g_combMtx = nullptr;	// ポインタをnullptrに設定
}

void Bones::Update() {
	auto transform = p_object->GetComponent<TransformComponent>();
	auto cameraobj = GameObjectManager::GameObjectFindName("camera");

	if (transform != nullptr && cameraobj != nullptr) {
		//定数バッファを更新
		CBBoneMatrix cb;

		cb.matrixWorld = transform->GetWorldMatrix();

		cb.color = DirectX::XMFLOAT4(m_Color);

		auto deviceContext = DirectXRender::GetDeviceContext();

		// 描画の処理
		// トポロジーをセット（プリミティブタイプ）
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 頂点の結び方の規則
		m_Shader->SetGPU();
		m_VertexBuffer.SetGPU();
		m_IndexBuffer.SetGPU();
		m_Texture->SetGPU();

		auto cameraComp = cameraobj->GetComponent<Camera>();

		cb.matrixView = cameraComp->GetViewMtx3D();
		cb.matrixProj = cameraComp->GetProjMtx3D();

		// 行列をシェーダーに渡す
		deviceContext->UpdateSubresource(g_pBoneConstantBuffer, 0, NULL, &cb, 0, 0);

		deviceContext->DrawIndexed(
			m_IndexBuffer.GetIndexSize(),							// 描画するインデックス数（立方体なんで36）
			0,							// 最初のインデックスバッファの位置
			0);

	}
}


std::vector<AnimationVertex> Bones::CreateBoneMeshVertices() {
	// 頂点データ

	m_boneVertices.resize(6);

	m_boneVertices[0].position = Vector3(-1.0f, -1.0f, 0.0f);
	m_boneVertices[1].position = Vector3(1.0f, -1.0f, 0.0f);
	m_boneVertices[2].position = Vector3(1.0f, 1.0f, 0.0f);
	m_boneVertices[3].position = Vector3(-1.0f, 1.0f, 0.0f);


	m_boneVertices[0].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_boneVertices[1].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_boneVertices[2].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_boneVertices[3].color = Color(1.0f, 1.0f, 1.0f, 1.0f);


	m_boneVertices[0].weight = Vector3(0.0f, 0.0f, 1.0f);	// 右下
	m_boneVertices[1].weight = Vector3(0.0f, 1.0f, 1.0f);	// 左下
	m_boneVertices[2].weight = Vector3(0.0f, 1.0f, 0.0f);	// 左上
	m_boneVertices[3].weight = Vector3(0.0f, 0.0f, 0.0f);	// 右上


	//m_boneVertices[0].normal = Vector3(0.0f, 0.0f, -1.0f);
	//m_boneVertices[1].normal = Vector3(0.0f, 0.0f, -1.0f);
	//m_boneVertices[2].normal = Vector3(0.0f, 0.0f, -1.0f);
	//m_boneVertices[3].normal = Vector3(0.0f, 0.0f, -1.0f);

	return m_boneVertices;
}

std::vector<unsigned int> Bones::CreateMeshIndices() {
	// インデックス情報生成

	m_indices.resize(6);

	m_indices = {
		2,1,0,
		0,3,2,
	};

	return m_indices;
}

void Bones::SetBoneMesh(Mesh& mesh) {
	m_AnimationVertexBuffer.Create(mesh.GetBoneVertices());
	m_IndexBuffer.Create(mesh.GetIndices());
}