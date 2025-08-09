#include "InchWorm_Test.h"
#include "Transform.h"
#include "GameObjectManager.h"
#include "DirectXRender.h"

using namespace DirectX::SimpleMath;

// ボーンのコンストラクタ
InchWorm_Test::InchWorm_Test(GameObject& obj) : RenderComponent(obj)
{
	// 頂点作成と重み付けもするよ
	m_sortNum = RENDER_ONE_SKIN_ANIMATION;
	m_Shader = std::make_unique<Shader>();
	m_Texture = std::make_unique<Texture>();

	CreateBoneMeshVertices();
	CreateMeshIndices();

	m_Shader->CreateOneSkinAnimation("OneSkinAnimationVS.hlsl", "OneSkinAnimationPS.hlsl");
	m_Texture->Load("assets/texture/NoTexture.png");

	BoneInit();
}

InchWorm_Test::~InchWorm_Test()
{
	delete[] m_bones;	// ボーンのメモリを解放
	m_bones = nullptr;	// ポインタをnullptrに設定

	delete[] g_combMtx;	// 合成変換行列のメモリを解放
	g_combMtx = nullptr;	// ポインタをnullptrに設定
}

void InchWorm_Test::Update() {

	GPU_Update();	// GPUで更新

	Draw();
}

void InchWorm_Test::GPU_Update() {
	AnimationVertex v[8];					// 頂点ブレンド計算した頂点群


	val += 0.03f;

	///////////////////////////////////////
	// ボーンの姿勢を更新
	//  最終的には、
	//   [ボーンオフセット行列] × [ワールド空間でのボーンの姿勢]
	//  を計算します。

	// ボーン行列作成（各ボーンの）
	// 各ボーンの初期姿勢からの差分姿勢（親空間ベース）を更新
	//  これは適当にぐにぐに動かして構わない部分です

	Matrix defBone[INCH_WORM_BONE_NUM];			// 2ボーン

	DX11MtxIdentity(defBone[0]);	// ルートボーンの姿勢を単位行列にする(動かない)

	Quaternion q = { 0.0f,0.0f,0.0f,1.0f };

	// 自ボーンの座標系でのボーンの姿勢を計算
	for (int i = 1; i < INCH_WORM_BONE_NUM; i++) {
		//		DX11MtxRotationY((sinf(val) * 70.0f), defBone[i]);
		q = Quaternion::CreateFromYawPitchRoll(sinf(val) * DirectX::XMConvertToRadians(70.0f), 0.0f, 0.0f);
		defBone[i] = Matrix::CreateFromQuaternion(q);	// 回転行列を作成
	}

	// ボーンの姿勢を自分の座標系基準に変換
	for (int i = 0; i < INCH_WORM_BONE_NUM; i++) {
		m_bones[i].boneMtx = defBone[i] * m_bones[i].initMtx;
	}

	/*
		// 再帰を使用した場合

		// 親空間ベースにある各ボーン行列をローカル空間ベースの姿勢に変換
		// ここは親子関係に従って行列を掛ける必要があります
		// 掛ける順番は 子 × 親 です。
		Matrix global;
		DX11MtxRotationZ(val, global);				// 中心をZ軸回転

		struct UpdateBone {
			static void run(Bone* me, Matrix *parentWorldMtx) {
				me->boneMtx = me->boneMtx*(*parentWorldMtx);			// ボーン行列＝ボーン行列×親の行列
				me->combMtxAry[me->id] = me->offsetMtx * me->boneMtx;
				if (me->firstChild)
					run(me->firstChild, &me->boneMtx);
				if (me->sibling)
					run(me->sibling, parentWorldMtx);
			};
		};
		UpdateBone::run(g_bones, &global);
	*/

	// 再帰を使用しなかった場合
	Matrix global;
	DX11MtxRotationZ(val * 1.0f, global);

	// ボーン行列を計算
	m_bones[0].boneMtx = m_bones[0].boneMtx/* * global*/;
	m_bones[1].boneMtx = m_bones[1].boneMtx * m_bones[0].boneMtx;
	//	m_bones[2].boneMtx = m_bones[2].boneMtx * m_bones[1].boneMtx;


		// ボーンコンビネーション行列を計算
	g_combMtx[0] = m_bones[0].offsetMtx * m_bones[0].boneMtx;
	g_combMtx[1] = m_bones[1].offsetMtx * m_bones[1].boneMtx;

	//auto debugCube = GameObjectManager::GameObjectFindName("joint0");
	//auto debugTrans = debugCube->GetComponent<TransformComponent>();

//	auto debugMtx = m_bones[1].boneMtx;

	//debugTrans->SetPosition({debugMtx._41,debugMtx._42 ,debugMtx._43 });
	//debugTrans->MakeWorldMatrix();

	//	g_combMtx[2] = m_bones[2].offsetMtx * m_bones[2].boneMtx;

	

		Vector3 scale, position;
	Quaternion rotation;

	if (m_bones[1].boneMtx.Decompose(scale, rotation, position)) {
		// 成功：回転だけ取り出せた
		Vector3 euler;

		// yaw (Y軸周り)
		float siny_cosp = 2.0f * (q.w * q.y + q.z * q.x);
		float cosy_cosp = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
		euler.y = std::atan2(siny_cosp, cosy_cosp); // yaw

		// pitch (X軸周り)
		float sinp = 2.0f * (q.w * q.x - q.z * q.y);
		if (std::abs(sinp) >= 1)
			euler.x = std::copysign(DirectX::XM_PIDIV2, sinp); // 90度
		else
			euler.x = std::asin(sinp); // pitch

		// roll (Z軸周り)
		float sinr_cosp = 2.0f * (q.w * q.z + q.x * q.y);
		float cosr_cosp = 1.0f - 2.0f * (q.z * q.z + q.x * q.x);
		euler.z = std::atan2(sinr_cosp, cosr_cosp); // roll

		// ラジアン → ディグリー変換
		euler.x = DirectX::XMConvertToDegrees(euler.x);
		euler.y = DirectX::XMConvertToDegrees(euler.y);
		euler.z = DirectX::XMConvertToDegrees(euler.z);

		/*debugTrans->SetRotation(euler);
		debugTrans->MakeWorldMatrix();*/
	}

		// デバッグ用に一応単位行列に
	for (int i = 0; i < INCH_WORM_BONE_NUM; i++) {
	//	g_combMtx[i] = Matrix::Identity;
	}


	// 転置
	for (int i = 0; i < INCH_WORM_BONE_NUM; i++) {
		DX11MtxTranspose(g_combMtx[i], g_combMtx[i]);
	}

	//auto transform = p_object->GetComponent<TransformComponent>();
	//auto srt = transform->GetWorldMatrix();

	//CBBoneMatrix cb;

	//// 定数バッファ更新
	//D3D11_MAPPED_SUBRESOURCE pData;
	//ID3D11DeviceContext* devcontext;

	//// デバイスコンテキスト取得
	//devcontext = DirectXRender::GetDeviceContext();

	//cb.matrixWorld = srt;		// ワールド変換行列

	//// ボーン行列配列
	//for (int i = 0; i < INCH_WORM_BONE_NUM; ++i) {
	//	cb.mtx[i] = g_combMtx[i];
	//}

	//// ボーン行列格納用定数バッファ更新
	//HRESULT hr = devcontext->Map(g_pBoneConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &pData);
	//if (SUCCEEDED(hr)) {
	//	memcpy_s(pData.pData, pData.RowPitch, &cb, sizeof(CBBoneMatrix));
	//	devcontext->Unmap(g_pBoneConstantBuffer, 0);
	//}

	//devcontext->VSSetConstantBuffers(8, 1, &g_pBoneConstantBuffer);
}

void InchWorm_Test::Draw() {
	auto transform = p_object->GetComponent<TransformComponent>();
	auto cameraobj = GameObjectManager::GameObjectFindName("camera");

	if (transform != nullptr && cameraobj != nullptr) {
		//定数バッファを更新
		CBBoneMatrix cb;

		cb.matrixWorld = transform->GetWorldMatrix().Transpose();

		//		cb.color = DirectX::XMFLOAT4(m_Color);

		auto deviceContext = DirectXRender::GetDeviceContext();

		// 描画の処理
		// トポロジーをセット（プリミティブタイプ）
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 頂点の結び方の規則
		m_Shader->SetGPU();

		m_AnimationVertexBuffer.SetGPU();

		m_IndexBuffer.SetGPU();
		m_Texture->SetGPU();

		//// デバッグ用に一応単位行列に
		//for (int i = 0; i < INCH_WORM_BONE_NUM; i++) {
		//	g_combMtx[i] = Matrix::Identity;
		//}


		//// 転置
		//for (int i = 0; i < INCH_WORM_BONE_NUM; i++) {
		//	DX11MtxTranspose(g_combMtx[i], g_combMtx[i]);
		//}

		cb.mtx[0] = g_combMtx[0];	// ボーン行列配列
		cb.mtx[1] = g_combMtx[1];	// ボーン行列配列


		// 行列をシェーダーに渡す
		deviceContext->UpdateSubresource(g_pBoneConstantBuffer, 0, NULL, &cb, 0, 0);

		deviceContext->DrawIndexed(
			m_IndexBuffer.GetIndexSize(),							// 描画するインデックス数（立方体なんで36）
			0,							// 最初のインデックスバッファの位置
			0);
	}
}

void InchWorm_Test::BoneInit() {
	m_bones = new Bone[INCH_WORM_BONE_NUM];	// ボーンの数だけメモリを確保

	// Tポーズ姿勢の初期化（回転）
	for (int i = 0; i < INCH_WORM_BONE_NUM; ++i) {
		m_bones[i].initMtx = Matrix::Identity;	// 全て単位行列に
	}

	// 親子関係を構築します
	m_bones[0].firstChild = &m_bones[1];	// 腰から胸
	//m_bones[1].firstChild = &m_bones[2];	// 胸から頭
	//m_bones[3].firstChild = &m_bones[4];	// 左肩から左腕

	// --- 回転設定 ---
	// 頭～腰：基本的にＺ軸 -90度回転（Ｙ軸上方向に立ち上がる）
	DX11MtxRotationZ(0.0f, m_bones[0].initMtx);	// 腰
	DX11MtxRotationZ(0.0f, m_bones[1].initMtx);	// 胸
	//	DX11MtxRotationZ(-90.0f, m_bones[2].initMtx);	// 頭


		// --- 位置設定 ---
		// 腰（ルート）＝ 原点
	m_bones[0].initMtx._41 = 0.0f;	// X座標
	m_bones[0].initMtx._42 = 0.0f;	// Y座標

	// 胸は腰の上（Ｙ方向）
	m_bones[1].initMtx._41 = 5.0f;	// X座標
	m_bones[1].initMtx._42 = 0.0f;	// Y座標
	// 頭は胸の上（Ｙ方向）
	//m_bones[2].initMtx._41 = 0.0f;	// X座標
	//m_bones[2].initMtx._42 = 1.0f;	// Y座標


	// 初期姿勢の影響を打ち消すために必要
	g_combMtx = new DirectX::SimpleMath::Matrix[INCH_WORM_BONE_NUM];	// 合成変換行列の配列を確保

	for (int i = 0; i < INCH_WORM_BONE_NUM; i++) {
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
	//	m_bones[2].initMtx = m_bones[2].initMtx * m_bones[1].offsetMtx;

}

std::vector<AnimationVertex> InchWorm_Test::CreateBoneMeshVertices() {
	// 頂点データ

	m_boneVertices.resize(8);

	m_boneVertices[0].position = Vector3(-1.0f, -1.0f, 0.0f);
	m_boneVertices[1].position = Vector3( 1.0f, -1.0f, 0.0f);
	m_boneVertices[2].position = Vector3( 1.0f,  1.0f, 0.0f);
	m_boneVertices[3].position = Vector3(-1.0f,  1.0f, 0.0f);

	m_boneVertices[4].position = Vector3(1.0f, -1.0f, 0.0f);
	m_boneVertices[5].position = Vector3(3.0f, -1.0f, 0.0f);
	m_boneVertices[6].position = Vector3(3.0f,  1.0f, 0.0f);
	m_boneVertices[7].position = Vector3(1.0f,  1.0f, 0.0f);


	m_boneVertices[0].color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	m_boneVertices[1].color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	m_boneVertices[2].color = Color(1.0f, 0.0f, 0.0f, 1.0f);
	m_boneVertices[3].color = Color(1.0f, 0.0f, 0.0f, 1.0f);

	m_boneVertices[4].color = Color(1.0f, 1.0f, 0.0f, 1.0f);
	m_boneVertices[5].color = Color(1.0f, 1.0f, 0.0f, 1.0f);
	m_boneVertices[6].color = Color(1.0f, 1.0f, 0.0f, 1.0f);
	m_boneVertices[7].color = Color(1.0f, 1.0f, 0.0f, 1.0f);


	m_boneVertices[0].weight = Vector3(1.00f, 0.00f, 0.00f);	// 右下
	m_boneVertices[1].weight = Vector3(1.00f, 0.00f, 0.00f);	// 左下
	m_boneVertices[2].weight = Vector3(1.00f, 0.00f, 0.00f);	// 左上
	m_boneVertices[3].weight = Vector3(1.00f, 0.00f, 0.00f);	// 右上
	m_boneVertices[4].weight = Vector3(1.00f, 0.00f, 0.00f);	// 左下
	m_boneVertices[5].weight = Vector3(0.30f, 0.70f, 0.00f);	// 右下
	m_boneVertices[6].weight = Vector3(0.30f, 0.70f, 0.00f);	// 右上
	m_boneVertices[7].weight = Vector3(1.00f, 0.00f, 0.00f);	// 左上


	m_boneVertices[0].matrixIndex[0] = 0;	m_boneVertices[0].matrixIndex[1] = 1;
	m_boneVertices[0].matrixIndex[2] = 0;	m_boneVertices[0].matrixIndex[3] = 0;

	m_boneVertices[1].matrixIndex[0] = 0;	m_boneVertices[1].matrixIndex[1] = 1;
	m_boneVertices[1].matrixIndex[2] = 0;	m_boneVertices[1].matrixIndex[3] = 0;

	m_boneVertices[2].matrixIndex[0] = 0;	m_boneVertices[2].matrixIndex[1] = 1;
	m_boneVertices[2].matrixIndex[2] = 0;	m_boneVertices[2].matrixIndex[3] = 0;

	m_boneVertices[3].matrixIndex[0] = 0;	m_boneVertices[3].matrixIndex[1] = 1;
	m_boneVertices[3].matrixIndex[2] = 0;	m_boneVertices[3].matrixIndex[3] = 0;

	m_boneVertices[4].matrixIndex[0] = 0;	m_boneVertices[4].matrixIndex[1] = 1;
	m_boneVertices[4].matrixIndex[2] = 0;	m_boneVertices[4].matrixIndex[3] = 0;

	m_boneVertices[5].matrixIndex[0] = 0;	m_boneVertices[5].matrixIndex[1] = 1;
	m_boneVertices[5].matrixIndex[2] = 0;	m_boneVertices[5].matrixIndex[3] = 0;

	m_boneVertices[6].matrixIndex[0] = 0;	m_boneVertices[6].matrixIndex[1] = 1;
	m_boneVertices[6].matrixIndex[2] = 0;	m_boneVertices[6].matrixIndex[3] = 0;

	m_boneVertices[7].matrixIndex[0] = 0;	m_boneVertices[7].matrixIndex[1] = 1;
	m_boneVertices[7].matrixIndex[2] = 0;	m_boneVertices[7].matrixIndex[3] = 0;



	//m_boneVertices[0].normal = Vector3(0.0f, 0.0f, -1.0f);
	//m_boneVertices[1].normal = Vector3(0.0f, 0.0f, -1.0f);
	//m_boneVertices[2].normal = Vector3(0.0f, 0.0f, -1.0f);
	//m_boneVertices[3].normal = Vector3(0.0f, 0.0f, -1.0f);

	m_AnimationVertexBuffer.Create(m_boneVertices);

	return m_boneVertices;
}

std::vector<unsigned int> InchWorm_Test::CreateMeshIndices() {
	// インデックス情報生成

	m_indices.resize(12);

	m_indices = {
		0,2,1,
		0,3,2,  // 左側の矩形 → 反時計回りに修正

		6,5,4,
		4,7,6   // 右側 → そのままでOK
	};


	m_IndexBuffer.Create(m_indices);

	return m_indices;
}

/*------------------------
X軸回転行列を求める
--------------------------*/
void InchWorm_Test::DX11MtxRotationX(float angle, DirectX::XMFLOAT4X4& outmtx) {

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
void InchWorm_Test::DX11MtxRotationY(float angle, DirectX::XMFLOAT4X4& outmtx) {

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
void InchWorm_Test::DX11MtxRotationZ(float angle, DirectX::XMFLOAT4X4& outmtx) {

	DirectX::XMMATRIX mtx;

	// 度数法をラジアンに変換
	angle = (angle * DirectX::XM_PI) / 180.0f;

	// 指定軸を中心に回転する行列を作成する
	mtx = DirectX::XMMatrixRotationZ(angle);

	XMStoreFloat4x4(&outmtx, mtx);
}

void InchWorm_Test::DX11MtxInverse(DirectX::XMFLOAT4X4& ansmtx, const DirectX::XMFLOAT4X4& mtx) {

	DirectX::XMMATRIX mat, matans;

	mat = XMLoadFloat4x4(&mtx);

	matans = DirectX::XMMatrixInverse(nullptr, mat);

	XMStoreFloat4x4(&ansmtx, matans);
}

void InchWorm_Test::DX11MtxIdentity(DirectX::XMFLOAT4X4& mat) {

	DirectX::XMMATRIX mtx;

	mtx = DirectX::XMMatrixIdentity();

	XMStoreFloat4x4(&mat, mtx);
}

void InchWorm_Test::DX11MtxTranspose(DirectX::XMFLOAT4X4& outmtx, const DirectX::XMFLOAT4X4& inmtx) {

	DirectX::XMMATRIX mtx;
	DirectX::XMMATRIX inmmtx;
	inmmtx = XMLoadFloat4x4(&inmtx);

	mtx = XMMatrixTranspose(inmmtx);

	XMStoreFloat4x4(&outmtx, mtx);
}