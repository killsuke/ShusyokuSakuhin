#include "MeshCut2DComponent.h"
#include "Mesh/SquareMesh.h"
#include "Render2D.h"
#include "Transform.h"
#include "VectorMoveComponent.h"
#include "input.h"
#include "Manager/GameObjectManager.h"
#include "System/DirectXRender.h"
#include "Manager/EventBusManager.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

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

	Render2DComponent* rendComp = m_Object->GetComponent<Render2DComponent>();
	if (rendComp == nullptr) {
		return;
	}

	Mesh* mesh = rendComp->GetMesh();
	if (mesh == nullptr) {
		return;
	}

	SquareMesh* squareMesh = dynamic_cast<SquareMesh*>(mesh);
	if (squareMesh == nullptr) {
		return;
	}

	Texture texture = rendComp->GetTexture();
	std::string texName = texture.GetTexname();
	const RightLeft isInversion = rendComp->GetInversionFlag();
	Shader* shader = rendComp->GetShader();
	std::vector<std::string> shaderName = shader->GetShaderNames();

	// 取り敢えず縦に左右に半分にカットする処理を書く
	// こんどは頂点バッファを書き換える処理を書く
	TransformComponent* trans = m_Object->GetComponent<TransformComponent>();
	Vector3 pos = trans->GetPosition();
	Vector3 size = trans->GetScale();

	m_CutObj1 = GameObjectManager::AddObject("CutLeft", "CutPart");
	TransformComponent* leftTrans = m_CutObj1->AddComponent<TransformComponent>();
	leftTrans->SetScale(Vector3(size.x, size.y, size.z));
	VectorMoveComponent* leftMove = m_CutObj1->AddComponent<VectorMoveComponent>();
	//leftMove->SetMoveDirection({ -1.0f,0.0f,0.0f });
	//leftMove->SetMovePower(0.1f);
	Render2DComponent* leftRend = m_CutObj1->AddComponent<Render2DComponent>();
	leftRend->CreateMesh<SquareMesh>();
	leftRend->SetShader(shaderName[0], shaderName[1], shaderName[2]);
	leftRend->ChangeTexture(texName);
	leftRend->SetInversionFlag(isInversion);
	m_CutObj1ID = m_CutObj1->GetInstanceID();

	m_CutObj2 = GameObjectManager::AddObject("CutRight", "CutPart");
	TransformComponent* rightTrans = m_CutObj2->AddComponent<TransformComponent>();
	rightTrans->SetScale(Vector3(size.x, size.y, size.z));
	VectorMoveComponent* rightMove = m_CutObj2->AddComponent<VectorMoveComponent>();
	//rightMove->SetMoveDirection({ 1.0f,0.0f,0.0f });
	//rightMove->SetMovePower(0.1f);
	Render2DComponent* rightRend = m_CutObj2->AddComponent<Render2DComponent>();
	rightRend->CreateMesh<SquareMesh>();
	rightRend->SetShader(shaderName[0], shaderName[1], shaderName[2]);
	rightRend->ChangeTexture(texName);
	rightRend->SetInversionFlag(isInversion);
	m_CutObj2ID = m_CutObj2->GetInstanceID();

	std::vector<VERTEX_3D> leftVertices;
	std::vector<VERTEX_3D> rightVertices;

	leftVertices.resize(4);
	rightVertices.resize(4);

	if (m_CutDirection == CutDirection::HORIZONTAL) { // HORIZONTAL

		leftTrans->SetPosition(Vector3(pos.x, pos.y + (size.y * 0.5f), pos.z));
		rightTrans->SetPosition(Vector3(pos.x, pos.y + (-size.y * 0.5f), pos.z));

		float vDLeft = 0.0f;
		float vDRight = 0.0f;
		float vTLeft = 0.0f;
		float vTRight = 0.0f;

		MakeCutPoints(vDLeft, vDRight, m_CutRatio1);
		MakeCutPoints(vTLeft, vTRight, m_CutRatio2);

		// 上の切れた方

		leftVertices[0].position = Vector3(-1.0f, vTRight, 0.0f);
		leftVertices[1].position = Vector3(1.0f, vDRight, 0.0f);
		leftVertices[2].position = Vector3(1.0f, 0.5f, 0.0f);
		leftVertices[3].position = Vector3(-1.0f, 0.5f, 0.0f);

		const float minYT = std::min(std::min(leftVertices[0].position.y,
			leftVertices[1].position.y),
			std::min(leftVertices[2].position.y,
				leftVertices[3].position.y));

		const float maxYT = std::max(std::max(leftVertices[0].position.y,
			leftVertices[1].position.y),
			std::max(leftVertices[2].position.y,
				leftVertices[3].position.y));

		// 4頂点分のuv値のyを計算
		for (int i = 0; i < 4; ++i) {
			const float px = leftVertices[i].position.y;
			const float t = 1 - ((px - minYT) / (maxYT - minYT)); // 0～1
			leftVertices[i].uv.y = t * m_CutRatio1;
		}

		leftVertices[0].uv.x = 0.0f;
		leftVertices[1].uv.x = 1.0f;
		leftVertices[2].uv.x = 1.0f;
		leftVertices[3].uv.x = 0.0f;

		// 下の切れた方
		rightVertices[0].position = Vector3(-1.0f, -0.5f, 0.0f);
		rightVertices[1].position = Vector3(1.0f, -0.5f, 0.0f);
		rightVertices[2].position = Vector3(1.0f, vDLeft, 0.0f);
		rightVertices[3].position = Vector3(-1.0f, vTLeft, 0.0f);

		float vL3 = leftVertices[2].uv.y;
		float vL4 = leftVertices[3].uv.y;

		// uv計算（上側のuv.yをスタートとする）
		rightVertices[0].uv.y = 1.0f;
		rightVertices[1].uv.y = 1.0f;
		rightVertices[2].uv.y = 0.5f - vL3;
		rightVertices[3].uv.y = 0.5f - vL4;

		rightVertices[0].uv.x = 0.0f;
		rightVertices[1].uv.x = 1.0f;
		rightVertices[2].uv.x = 1.0f;
		rightVertices[3].uv.x = 0.0f;

	}
	else {// VERTICAL
		leftTrans->SetPosition(Vector3(pos.x + (-size.x * 0.5f), pos.y, pos.z));
		rightTrans->SetPosition(Vector3(pos.x + (size.x * 0.5f), pos.y, pos.z));

		float vLTop = 0.0f;
		float vRTop = 0.0f;
		float vLDown = 0.0f;
		float vRDown = 0.0f;

		MakeCutPoints(vLTop, vRTop, m_CutRatio1);
		MakeCutPoints(vLDown, vRDown, m_CutRatio2);

		// 左側の切れた方
		leftVertices[0].position = Vector3(-0.5f, -1.0f, 0.0f);
		leftVertices[1].position = Vector3(vLDown, -1.0f, 0.0f);
		leftVertices[2].position = Vector3(vLTop, 1.0f, 0.0f);
		leftVertices[3].position = Vector3(-0.5f, 1.0f, 0.0f);

		const float minXL = std::min(std::min(leftVertices[0].position.x,
			leftVertices[1].position.x),
			std::min(leftVertices[2].position.x,
				leftVertices[3].position.x));

		const float maxXL = std::max(std::max(leftVertices[0].position.x,
			leftVertices[1].position.x),
			std::max(leftVertices[2].position.x,
				leftVertices[3].position.x));

		// 4頂点分のuv値のxを計算
		for (int i = 0; i < 4; ++i) {
			const float px = leftVertices[i].position.x;
			const float t = (px - minXL) / (maxXL - minXL); // 0～1
			leftVertices[i].uv.x = t * m_CutRatio1;
		}

		leftVertices[0].uv.y = 1.0f;
		leftVertices[1].uv.y = 1.0f;
		leftVertices[2].uv.y = 0.0f;
		leftVertices[3].uv.y = 0.0f;

		// 右側の切れた方
		rightVertices[0].position = Vector3(vRDown, -1.0f, 0.0f);
		rightVertices[1].position = Vector3(0.5f, -1.0f, 0.0f);
		rightVertices[2].position = Vector3(0.5f, 1.0f, 0.0f);
		rightVertices[3].position = Vector3(vRTop, 1.0f, 0.0f);

		float vL2 = leftVertices[1].uv.x;
		float vL3 = leftVertices[2].uv.x;

		// uv計算（左側のuv.xをスタートとする）
		rightVertices[0].uv.x = 1.0f - vL3;
		rightVertices[1].uv.x = 1.0f;
		rightVertices[2].uv.x = 1.0f;
		rightVertices[3].uv.x = 1.0f - vL2;

		rightVertices[0].uv.y = 1.0f;
		rightVertices[1].uv.y = 1.0f;
		rightVertices[2].uv.y = 0.0f;
		rightVertices[3].uv.y = 0.0f;
	}

	leftVertices[0].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	leftVertices[1].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	leftVertices[2].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	leftVertices[3].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	leftVertices[0].normal = Vector3(0.0f, 0.0f, -1.0f);
	leftVertices[1].normal = Vector3(0.0f, 0.0f, -1.0f);
	leftVertices[2].normal = Vector3(0.0f, 0.0f, -1.0f);
	leftVertices[3].normal = Vector3(0.0f, 0.0f, -1.0f);

	rightVertices[0].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	rightVertices[1].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	rightVertices[2].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	rightVertices[3].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	rightVertices[0].normal = Vector3(0.0f, 0.0f, -1.0f);
	rightVertices[1].normal = Vector3(0.0f, 0.0f, -1.0f);
	rightVertices[2].normal = Vector3(0.0f, 0.0f, -1.0f);
	rightVertices[3].normal = Vector3(0.0f, 0.0f, -1.0f);

	VertexBuffer<VERTEX_3D>* vLBuffer = leftRend->GetVertexBuffer();
	vLBuffer->Modify(leftVertices);

	VertexBuffer<VERTEX_3D>* vRBuffer = rightRend->GetVertexBuffer();
	vRBuffer->Modify(rightVertices);
}