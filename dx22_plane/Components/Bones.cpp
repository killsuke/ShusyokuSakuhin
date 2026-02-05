#include "Bones.h"
#include "Transform.h"
#include "Manager/GameObjectManager.h"
#include "Camera.h"
using namespace DirectX::SimpleMath;

// ボーンのコンストラクタ
Bones::Bones(GameObject& obj) : RenderComponent(obj)
{
	// 頂点作成と重み付けもするよ
	m_SortNum = ComponentTypeManager::GetID_FromName("RENDER_ONE_SKIN_ANIMATION"); // ソート番号を設定
	m_Shader = std::make_unique<Shader>();
	//m_Texture = std::make_unique<Texture>();

	m_bones = new Bone[BONE_NUM];	// ボーンの数だけメモリを確保

	// Tポーズ姿勢の初期化（回転）
	for(int i = 0; i < BONE_NUM; ++i) {
		m_bones[i].initMtx = Matrix::Identity;	// 全て単位行列に
	}

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

	// --- 回転設定 ---
	// 頭～腰：基本的にＺ軸 -90度回転（Ｙ軸上方向に立ち上がる）
	DX11MtxRotationZ(-90.0f, m_bones[0].initMtx);	// 腰
	DX11MtxRotationZ(-90.0f, m_bones[1].initMtx);	// 胸
	DX11MtxRotationZ(-90.0f, m_bones[2].initMtx);	// 頭

	// 肩と腕：Tポーズなので左右に水平
	DX11MtxRotationZ(0.0f, m_bones[3].initMtx);	// 左肩
	DX11MtxRotationZ(0.0f, m_bones[4].initMtx);	// 左腕
	DX11MtxRotationZ(0.0f, m_bones[5].initMtx);	// 左手

	DX11MtxRotationZ(0.0f, m_bones[6].initMtx);	// 右肩
	DX11MtxRotationZ(0.0f, m_bones[7].initMtx);	// 右腕
	DX11MtxRotationZ(0.0f, m_bones[8].initMtx);	// 右手

	// 足：立っている姿勢
	DX11MtxRotationX(-90.0f, m_bones[9].initMtx);	// 左足付け根
	DX11MtxRotationX(-90.0f, m_bones[10].initMtx);	// 左膝
	DX11MtxRotationX(-90.0f, m_bones[11].initMtx);	// 左足首

	DX11MtxRotationX(-90.0f, m_bones[12].initMtx);	// 右足付け根
	DX11MtxRotationX(-90.0f, m_bones[13].initMtx);	// 右膝
	DX11MtxRotationX(-90.0f, m_bones[14].initMtx);	// 右足首

	// --- 位置設定 ---
	// 腰（ルート）＝ 原点
	m_bones[0].initMtx._41 = 0.0f;	// X座標
	m_bones[0].initMtx._42 = 0.0f;	// Y座標

	// 胸は腰の上（Ｙ方向）
	m_bones[1].initMtx._41 = 0.0f;	// X座標
	m_bones[1].initMtx._42 = 1.0f;	// Y座標
	// 頭は胸の上（Ｙ方向）
	m_bones[2].initMtx._41 = 0.0f;	// X座標
	m_bones[2].initMtx._42 = 1.0f;	// Y座標

	// 左肩 → 左腕 → 左手（Ｘ方向に左へ）
	m_bones[3].initMtx._41 = -0.5f;	// X座標
	m_bones[3].initMtx._42 =  0.0f;	// Y座標

	m_bones[4].initMtx._41 = -1.0f;	// X座標
	m_bones[4].initMtx._42 =  0.0f;	// Y座標

	m_bones[5].initMtx._41 = -1.0f;	// X座標
	m_bones[5].initMtx._42 =  0.0f;	// Y座標

	// 右肩 → 右腕 → 右手（Ｘ方向に右へ）
	m_bones[6].initMtx._41 = 0.5f;	// X座標
	m_bones[6].initMtx._42 = 0.0f;	// Y座標

	m_bones[7].initMtx._41 = 1.0f;	// X座標
	m_bones[7].initMtx._42 = 0.0f;	// Y座標

	m_bones[8].initMtx._41 = 1.0f;	// X座標
	m_bones[8].initMtx._42 = 0.0f;	// Y座標

	// 足：腰から下にＹ方向
	m_bones[9].initMtx._41 = -0.3f;	// X座標
	m_bones[9].initMtx._42 = -1.0f;	// Y座標

	m_bones[10].initMtx._41 = 0.0f;	// X座標
	m_bones[10].initMtx._42 = -1.0f;	// Y座標

	m_bones[11].initMtx._41 = 0.0f;	// X座標
	m_bones[11].initMtx._42 = -1.0f;	// Y座標


	// 初期姿勢の影響を打ち消すために必要
	g_combMtx = new DirectX::SimpleMath::Matrix[BONE_NUM];	// 合成変換行列の配列を確保

	for (int i = 0; i < BONE_NUM; i++) {
		m_bones[i].id = i;
		m_bones[i].combMtxAry = g_combMtx;
		// ボーンオフセット行列作成（初期姿勢の逆行列計算）
		DX11MtxInverse(m_bones[i].offsetMtx, m_bones[i].initMtx);
	}

	/*
//	再帰を使用した場合
	// 初期姿勢を親の姿勢からの相対姿勢に直します。
	// まず子の末端まで下りて、自分のローカル空間での初期姿勢 × 親のボーンオフセット行列で相対姿勢が出ます
	// 親子関係を辿るので再起関数が必要です。

	struct CalcRelativeMat {
		static void run(Bone* me, Matrix *parentoffsetMat) {
			if (me->firstChild)
				run(me->firstChild, &me->offsetMtx);
			if (me->sibling)
				run(me->sibling, parentoffsetMat);
			if (parentoffsetMat)
				me->initMtx *= *parentoffsetMat;
		}
	};
	CalcRelativeMat::run(g_bones, 0);

*/
// 再帰を使わない場合
//   ボーンの初期姿勢を親ボーンからの相対座標系に変換する
	// ようするに親子関係を作る
	m_bones[0].initMtx = m_bones[0].initMtx;
	m_bones[1].initMtx = m_bones[1].initMtx * m_bones[0].offsetMtx;
	m_bones[2].initMtx = m_bones[2].initMtx * m_bones[1].offsetMtx;

	m_bones[3].initMtx = m_bones[3].initMtx * m_bones[1].offsetMtx;
	m_bones[4].initMtx = m_bones[4].initMtx * m_bones[3].offsetMtx;
	m_bones[5].initMtx = m_bones[5].initMtx * m_bones[4].offsetMtx;

	m_bones[6].initMtx = m_bones[6].initMtx * m_bones[1].offsetMtx;
	m_bones[7].initMtx = m_bones[7].initMtx * m_bones[6].offsetMtx;
	m_bones[8].initMtx = m_bones[8].initMtx * m_bones[7].offsetMtx;

	m_bones[9].initMtx = m_bones[9].initMtx * m_bones[0].offsetMtx;
	m_bones[10].initMtx = m_bones[10].initMtx * m_bones[9].offsetMtx;
	m_bones[11].initMtx = m_bones[11].initMtx * m_bones[10].offsetMtx;

	m_bones[12].initMtx = m_bones[12].initMtx * m_bones[0].offsetMtx;
	m_bones[13].initMtx = m_bones[13].initMtx * m_bones[12].offsetMtx;
	m_bones[14].initMtx = m_bones[14].initMtx * m_bones[13].offsetMtx;

}


Bones::~Bones()
{
	delete[] m_bones;	// ボーンのメモリを解放
	m_bones = nullptr;	// ポインタをnullptrに設定

	delete[] g_combMtx;	// 合成変換行列のメモリを解放
	g_combMtx = nullptr;	// ポインタをnullptrに設定
}

void Bones::Update() {
	auto transform = m_Object->GetComponent<TransformComponent>();
	auto cameraobj = GameObjectManager::GameObjectFindName("camera");

	if (transform != nullptr && cameraobj != nullptr) {
		//定数バッファを更新
		CBBoneMatrix cb;

		//cb.matrixWorld = transform->GetWorldMatrix();

		//cb.color = DirectX::XMFLOAT4(m_Color);

		auto deviceContext = DirectXRender::GetDeviceContext();

		// 描画の処理
		// トポロジーをセット（プリミティブタイプ）
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 頂点の結び方の規則
		m_Shader->SetGPU();
		m_VertexBuffer.SetGPU();
		m_IndexBuffer.SetGPU();
		//m_Texture->SetGPU();

		auto cameraComp = cameraobj->GetComponent<Camera>();

	/*	cb.matrixView = cameraComp->GetViewMtx3D();
		cb.matrixProj = cameraComp->GetProjMtx3D();*/

		ID3D11Buffer* buffer = DirectXRender::GetBoneBuffer();

		// 行列をシェーダーに渡す
		deviceContext->UpdateSubresource(buffer, 0, NULL, &cb, 0, 0);

		deviceContext->DrawIndexed(
			m_IndexBuffer.GetIndexSize(),							// 描画するインデックス数（立方体なんで36）
			0,							// 最初のインデックスバッファの位置
			0);

	}
}


std::vector<AnimationVertex> Bones::CreateBoneMeshVertices() {
	// 頂点データ

	m_boneVertices.resize(6);

	/*m_boneVertices[0].position = Vector3(-1.0f, -1.0f, 0.0f);
	m_boneVertices[1].position = Vector3(1.0f, -1.0f, 0.0f);
	m_boneVertices[2].position = Vector3(1.0f, 1.0f, 0.0f);
	m_boneVertices[3].position = Vector3(-1.0f, 1.0f, 0.0f);*/


	/*m_boneVertices[0].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_boneVertices[1].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_boneVertices[2].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_boneVertices[3].color = Color(1.0f, 1.0f, 1.0f, 1.0f);*/


	//m_boneVertices[0].weight = Vector3(0.0f, 0.0f, 1.0f);	// 右下
	//m_boneVertices[1].weight = Vector3(0.0f, 1.0f, 1.0f);	// 左下
	//m_boneVertices[2].weight = Vector3(0.0f, 1.0f, 0.0f);	// 左上
	//m_boneVertices[3].weight = Vector3(0.0f, 0.0f, 0.0f);	// 右上


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
	// 頂点とインデックスデータを作る
	/*m_AnimationVertexBuffer.Create(mesh.GetBoneVertices());
	m_IndexBuffer.Create(mesh.GetIndices());*/
}

/*------------------------
X軸回転行列を求める
--------------------------*/
void Bones::DX11MtxRotationX(float angle, DirectX::XMFLOAT4X4& outmtx) {

	DirectX::XMMATRIX mtx;

	// 度数法をラジアンに変換
	angle = (angle * DirectX::XM_PI) / 180.0f;

	// X軸を中心に回転する行列を作成する
	mtx = DirectX::XMMatrixRotationX(angle);

	XMStoreFloat4x4(&outmtx, mtx);
}

/*------------------------
  Y軸回転行列を求める
--------------------------*/
void Bones::DX11MtxRotationY(float angle, DirectX::XMFLOAT4X4& outmtx) {

	DirectX::XMMATRIX mtx;

	// 度数法をラジアンに変換
	angle = (angle * DirectX::XM_PI) / 180.0f;

	// Y軸を中心に回転する行列を作成する
	mtx = DirectX::XMMatrixRotationY(angle);

	XMStoreFloat4x4(&outmtx, mtx);
}

/*------------------------
   Z軸回転行列を求める
--------------------------*/
void Bones::DX11MtxRotationZ(float angle, DirectX::XMFLOAT4X4& outmtx) {

	DirectX::XMMATRIX mtx;

	// 度数法をラジアンに変換
	angle = (angle * DirectX::XM_PI) / 180.0f;

	// 指定軸を中心に回転する行列を作成する
	mtx = DirectX::XMMatrixRotationZ(angle);

	XMStoreFloat4x4(&outmtx, mtx);
}

void Bones::DX11MtxInverse(DirectX::XMFLOAT4X4& ansmtx, const DirectX::XMFLOAT4X4& mtx) {

	DirectX::XMMATRIX mat, matans;

	mat = XMLoadFloat4x4(&mtx);

	matans = DirectX::XMMatrixInverse(nullptr, mat);

	XMStoreFloat4x4(&ansmtx, matans);
}