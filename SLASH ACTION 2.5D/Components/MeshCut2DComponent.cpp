#include "MeshCut2DComponent.h"
#include "Mesh/SquareMesh.h"
#include "Render2DComponent.h"
#include "TransformComponent.h"
#include "VectorMoveComponent.h"
#include "Input/Input.h"
#include "Manager/GameObjectManager.h"
#include "System/DirectXRender.h"
#include "Manager/EventBusManager.h"
#include <array>

using namespace DirectX;

MeshCut2DComponent::MeshCut2DComponent(GameObject& obj) : Component(obj)
{
	// コンポーネントのソート番号を設定（あとで）
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER_CONTOROL"); // ソート番号を設定

	m_listenerID_CutEvent = EventBusManager::Subscribe<CutEvent>([&](const CutEvent& e) {
		MeshCutAction(e);
		});
}

MeshCut2DComponent::~MeshCut2DComponent() {
	EventBusManager::Unsubscribe(m_listenerID_CutEvent);
}

void MeshCut2DComponent::Update()
{

}

// 切断後の頂点の位置計算
void MeshCut2DComponent::MakeCutPoints(float& vL, float& vR, const float ratio) {

	const float value = 2.0f * ratio;

	// 左から見てどれぐらいの位置か
	vL = value;
	const float value2 = 2.0f - value;

	// ポジション計算用に調整
	vL -= 0.5f;
	vR = 0.5f - value2;
}

void MeshCut2DComponent::MeshCutAction(const CutEvent& event) {

	const uint32_t id = m_Object->GetInstanceID();

	if (event.targetID != id) {
		return; // 自分宛じゃないなら無視
	}

	m_CutDirection = event.cutDirection;
	m_CutRatio1 = event.ratio1;
	m_CutRatio2 = event.ratio2;

	MakeMeshCutAction(m_CutDirection, m_CutRatio1, m_CutRatio2);
}

std::array<uint32_t, 2> MeshCut2DComponent::MakeMeshCutAction(const CutDirection& dir, const float raito1, const float ratio2) {

	std::array<uint32_t, 2> cutObjIDs;
	cutObjIDs = { 0,0 };
	
	// Render2Dは3Dと統合してもいいかもしれん
	Render2DComponent* rendComp = m_Object->GetComponent<Render2DComponent>();
	if (rendComp == nullptr) {
		return cutObjIDs;
	}

	Mesh* mesh = rendComp->GetMesh();
	if (mesh == nullptr) {
		return cutObjIDs;
	}

	const XMFLOAT4 uvs = mesh->GetUVSets();

	SquareMesh* squareMesh = dynamic_cast<SquareMesh*>(mesh);
	if (squareMesh == nullptr) {
		return cutObjIDs;
	}

	// テクスチャの情報取得
	Texture texture = rendComp->GetTexture();
	std::string texName = texture.GetTexname();
	const RightLeft isInversion = rendComp->GetInversionFlag();
	Shader* shader = rendComp->GetShader();
	TransformComponent* trans = m_Object->GetComponent<TransformComponent>();
	XMFLOAT3 pos = trans->GetPosition();
	XMFLOAT3 size = trans->GetScale();

	// 切断後のオブジェクトを生成
	// オブジェクト１
	m_CutObj1 = GameObjectManager::AddObject("CutLeft", "CutPart");
	TransformComponent* leftTrans = m_CutObj1->AddComponent<TransformComponent>();
	leftTrans->SetScale(XMFLOAT3(size.x, size.y, size.z));
	VectorMoveComponent* leftMove = m_CutObj1->AddComponent<VectorMoveComponent>();
	Render2DComponent* leftRend = m_CutObj1->AddComponent<Render2DComponent>();
	Mesh* leftMesh = leftRend->CreateMesh<SquareMesh>();
	leftRend->SetShader("Animation2DVS.hlsl", "unlitTexturePS.hlsl");
	leftRend->ChangeTexture(texName);
	leftRend->SetInversionFlag(isInversion);
	leftMesh->SetTextureUV(uvs.x, uvs.y, uvs.z, uvs.w);

	m_CutObj1ID = m_CutObj1->GetInstanceID();

	// オブジェクト２
	m_CutObj2 = GameObjectManager::AddObject("CutRight", "CutPart");
	TransformComponent* rightTrans = m_CutObj2->AddComponent<TransformComponent>();
	rightTrans->SetScale(XMFLOAT3(size.x, size.y, size.z));
	VectorMoveComponent* rightMove = m_CutObj2->AddComponent<VectorMoveComponent>();
	Render2DComponent* rightRend = m_CutObj2->AddComponent<Render2DComponent>();
	Mesh* rightMesh = rightRend->CreateMesh<SquareMesh>();
	rightRend->SetShader("Animation2DVS.hlsl", "unlitTexturePS.hlsl");
	rightRend->ChangeTexture(texName);
	rightRend->SetInversionFlag(isInversion);
	rightMesh->SetTextureUV(uvs.x, uvs.y, uvs.z, uvs.w);

	m_CutObj2ID = m_CutObj2->GetInstanceID();

	std::vector<VERTEX_3D> CutVertices1;
	std::vector<VERTEX_3D> CutVertices2;

	CutVertices1.resize(4);
	CutVertices2.resize(4);

	// 切断がどちらの方向か決定
	// 頂点とUVの位置を計算してセット
	if (m_CutDirection == CutDirection::HORIZONTAL) { // HORIZONTAL

		leftTrans->SetPosition(XMFLOAT3(pos.x, pos.y + (size.y * 0.5f), pos.z));
		rightTrans->SetPosition(XMFLOAT3(pos.x, pos.y + (-size.y * 0.5f), pos.z));

		float vDLeft = 0.0f;
		float vDRight = 0.0f;
		float vTLeft = 0.0f;
		float vTRight = 0.0f;

		MakeCutPoints(vDLeft, vDRight, m_CutRatio1);
		MakeCutPoints(vTLeft, vTRight, m_CutRatio2);

		// 上側 ====================================================
		CutVertices1[0].position = XMFLOAT3(-1.0f, vTRight, 0.0f);
		CutVertices1[1].position = XMFLOAT3(1.0f, vDRight, 0.0f);
		CutVertices1[2].position = XMFLOAT3(1.0f, 0.5f, 0.0f);
		CutVertices1[3].position = XMFLOAT3(-1.0f, 0.5f, 0.0f);

		CutVertices1[0].uv = { 0.0f, m_CutRatio1 };
		CutVertices1[1].uv = { 1.0f, m_CutRatio2 };
		CutVertices1[2].uv = { 1.0f, 0.0f };
		CutVertices1[3].uv = { 0.0f, 0.0f };
		// =========================================================

		// 下側 ====================================================
		CutVertices2[0].position = XMFLOAT3(-1.0f, -0.5f, 0.0f);
		CutVertices2[1].position = XMFLOAT3(1.0f, -0.5f, 0.0f);
		CutVertices2[2].position = XMFLOAT3(1.0f, vDLeft, 0.0f);
		CutVertices2[3].position = XMFLOAT3(-1.0f, vTLeft, 0.0f);

		CutVertices2[0].uv = { 0.0f, 1.0f };
		CutVertices2[1].uv = { 1.0f, 1.0f };
		CutVertices2[2].uv = { 1.0f, m_CutRatio2 };
		CutVertices2[3].uv = { 0.0f, m_CutRatio1 };
		// =========================================================
	}
	else {// VERTICAL
		leftTrans->SetPosition(XMFLOAT3(pos.x + (-size.x * 0.5f), pos.y, pos.z));
		rightTrans->SetPosition(XMFLOAT3(pos.x + (size.x * 0.5f), pos.y, pos.z));

		float vLTop = 0.0f;
		float vRTop = 0.0f;
		float vLDown = 0.0f;
		float vRDown = 0.0f;

		MakeCutPoints(vLTop, vRTop, m_CutRatio1);
		MakeCutPoints(vLDown, vRDown, m_CutRatio2);

		// 左側 ====================================================
		CutVertices1[0].position = XMFLOAT3(-0.5f, -1.0f, 0.0f);
		CutVertices1[1].position = XMFLOAT3(vLDown, -1.0f, 0.0f);
		CutVertices1[2].position = XMFLOAT3(vLTop, 1.0f, 0.0f);
		CutVertices1[3].position = XMFLOAT3(-0.5f, 1.0f, 0.0f);

		CutVertices1[0].uv = { 0.0f, 1.0f };
		CutVertices1[1].uv = { m_CutRatio1, 1.0f };
		CutVertices1[2].uv = { m_CutRatio2, 0.0f };
		CutVertices1[3].uv = { 0.0f, 0.0f };
		// =========================================================

		// 右側 ====================================================
		CutVertices2[0].position = XMFLOAT3(vRDown, -1.0f, 0.0f);
		CutVertices2[1].position = XMFLOAT3(0.5f, -1.0f, 0.0f);
		CutVertices2[2].position = XMFLOAT3(0.5f, 1.0f, 0.0f);
		CutVertices2[3].position = XMFLOAT3(vRTop, 1.0f, 0.0f);

		CutVertices2[0].uv = { m_CutRatio1, 1.0f };
		CutVertices2[1].uv = { 1.0f, 1.0f };
		CutVertices2[2].uv = { 1.0f,  0.0f };
		CutVertices2[3].uv = { m_CutRatio2, 0.0f };
		// =========================================================
	}

	// 他の情報を設定

	CutVertices1[0].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	CutVertices1[1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	CutVertices1[2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	CutVertices1[3].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	CutVertices1[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	CutVertices1[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	CutVertices1[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	CutVertices1[3].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	CutVertices2[0].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	CutVertices2[1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	CutVertices2[2].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	CutVertices2[3].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	CutVertices2[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	CutVertices2[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	CutVertices2[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	CutVertices2[3].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// 頂点の情報をセット
	VertexBuffer<VERTEX_3D>* vLBuffer = leftRend->GetVertexBuffer();
	vLBuffer->Modify(CutVertices1);

	VertexBuffer<VERTEX_3D>* vRBuffer = rightRend->GetVertexBuffer();
	vRBuffer->Modify(CutVertices2);

	cutObjIDs[0] = m_CutObj1ID;
	cutObjIDs[1] = m_CutObj2ID;

	return cutObjIDs;
}