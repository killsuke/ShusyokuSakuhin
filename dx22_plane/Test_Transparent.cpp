#include "Test_Transparent.h"
#include "input.h"
#include "TestBoard.h"
#include "TestCube.h"
#include "Game.h"
using namespace DirectX::SimpleMath;

// コンストラクタ
Test_Transparent::Test_Transparent(Camera* cam) :GameObject(cam) {
}

// デストラクタ
Test_Transparent::~Test_Transparent() {

}

//=======================================
//初期化処理
//=======================================
void Test_Transparent::Init()
{
	// 頂点データ
	std::vector<VERTEX_3D>	vertices;

	vertices.resize(24);

	vertices[0].position = Vector3(-1.0f, 1.0f, 1.0f);
	vertices[1].position = Vector3(1.0f, 1.0f, 1.0f);
	vertices[2].position = Vector3(-1.0f, 1.0f, -1.0f);
	vertices[3].position = Vector3(1.0f, 1.0f, -1.0f);

	vertices[0].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[1].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[2].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[3].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	vertices[0].uv = Vector2(0.0f, 0.0f);
	vertices[1].uv = Vector2(0.33f, 0.0f);
	vertices[2].uv = Vector2(0.0f, 0.5f);
	vertices[3].uv = Vector2(0.33f, 0.5f);

	vertices[0].normal = Vector3(0.0f, 1.0f, 0.0f);
	vertices[1].normal = Vector3(0.0f, 1.0f, 0.0f);
	vertices[2].normal = Vector3(0.0f, 1.0f, 0.0f);
	vertices[3].normal = Vector3(0.0f, 1.0f, 0.0f);


	vertices[4].position = Vector3(-1.0f, 1.0f, -1.0f);
	vertices[5].position = Vector3(1.0f, 1.0f, -1.0f);
	vertices[6].position = Vector3(-1.0f, -1.0f, -1.0f);
	vertices[7].position = Vector3(1.0f, -1.0f, -1.0f);

	vertices[4].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[5].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[6].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[7].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	vertices[4].uv = Vector2(0.33f, 0.0f);
	vertices[5].uv = Vector2(0.66f, 0.0f);
	vertices[6].uv = Vector2(0.33f, 0.5f);
	vertices[7].uv = Vector2(0.66f, 0.5f);

	vertices[4].normal = Vector3(0.0f, 0.0f, -1.0f);
	vertices[5].normal = Vector3(0.0f, 0.0f, -1.0f);
	vertices[6].normal = Vector3(0.0f, 0.0f, -1.0f);
	vertices[7].normal = Vector3(0.0f, 0.0f, -1.0f);


	vertices[8].position = Vector3(1.0f, -1.0f, 1.0f);
	vertices[9].position = Vector3(1.0f, 1.0f, 1.0f);
	vertices[10].position = Vector3(-1.0f, -1.0f, 1.0f);
	vertices[11].position = Vector3(-1.0f, 1.0f, 1.0f);

	vertices[8].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[9].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[10].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[11].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	vertices[8].uv = Vector2(0.33f, 0.5f);
	vertices[9].uv = Vector2(0.66f, 0.5f);
	vertices[10].uv = Vector2(0.33f, 1.0f);
	vertices[11].uv = Vector2(0.66f, 1.0f);

	vertices[8].normal = Vector3(0.0f, 0.0f, 1.0f);
	vertices[9].normal = Vector3(0.0f, 0.0f, 1.0f);
	vertices[10].normal = Vector3(0.0f, 0.0f, 1.0f);
	vertices[11].normal = Vector3(0.0f, 0.0f, 1.0f);


	vertices[12].position = Vector3(-1.0f, -1.0f, 1.0f);
	vertices[13].position = Vector3(-1.0f, 1.0f, 1.0f);
	vertices[14].position = Vector3(-1.0f, -1.0f, -1.0f);
	vertices[15].position = Vector3(-1.0f, 1.0f, -1.0f);

	vertices[12].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[13].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[14].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[15].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	vertices[12].uv = Vector2(0.66f, 0.0f);
	vertices[13].uv = Vector2(1.0f, 0.0f);
	vertices[14].uv = Vector2(0.66f, 0.5f);
	vertices[15].uv = Vector2(1.0f, 0.5f);

	vertices[12].normal = Vector3(-1.0f, 0.0f, 0.0f);
	vertices[13].normal = Vector3(-1.0f, 0.0f, 0.0f);
	vertices[14].normal = Vector3(-1.0f, 0.0f, 0.0f);
	vertices[15].normal = Vector3(-1.0f, 0.0f, 0.0f);


	vertices[16].position = Vector3(1.0f, -1.0f, -1.0f);
	vertices[17].position = Vector3(1.0f, 1.0f, -1.0f);
	vertices[18].position = Vector3(1.0f, -1.0f, 1.0f);
	vertices[19].position = Vector3(1.0f, 1.0f, 1.0f);

	vertices[16].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[17].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[18].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[19].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	vertices[16].uv = Vector2(0.0f, 0.5f);
	vertices[17].uv = Vector2(0.33f, 0.5f);
	vertices[18].uv = Vector2(0.0f, 1.0f);
	vertices[19].uv = Vector2(0.33f, 1.0f);

	vertices[16].normal = Vector3(1.0f, 0.0f, 0.0f);
	vertices[17].normal = Vector3(1.0f, 0.0f, 0.0f);
	vertices[18].normal = Vector3(1.0f, 0.0f, 0.0f);
	vertices[19].normal = Vector3(1.0f, 0.0f, 0.0f);


	vertices[20].position = Vector3(-1.0f, -1.0f, 1.0f);
	vertices[21].position = Vector3(1.0f, -1.0f, 1.0f);
	vertices[22].position = Vector3(-1.0f, -1.0f, -1.0f);
	vertices[23].position = Vector3(1.0f, -1.0f, -1.0f);

	vertices[20].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[21].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[22].color = Color(1.0f, 1.0f, 1.0f, 1.0f);
	vertices[23].color = Color(1.0f, 1.0f, 1.0f, 1.0f);

	vertices[20].uv = Vector2(0.66f, 0.5f);
	vertices[21].uv = Vector2(1.0f, 0.5f);
	vertices[22].uv = Vector2(0.66f, 1.0f);
	vertices[23].uv = Vector2(1.0f, 1.0f);

	vertices[20].normal = Vector3(0.0f, -1.0f, 0.0f);
	vertices[21].normal = Vector3(0.0f, -1.0f, 0.0f);
	vertices[22].normal = Vector3(0.0f, -1.0f, 0.0f);
	vertices[23].normal = Vector3(0.0f, -1.0f, 0.0f);

	// 頂点バッファ生成
	m_VertexBuffer.Create(vertices);

	// インデックスバッファ生成
	std::vector<unsigned int> indices;
	indices.resize(36);

	indices = {
		0,1,2,
		1,3,2,

		4,5,6,
		5,7,6,

		8,9,10,
		9,11,10,

		12,13,14,
		13,15,14,

		16,17,18,
		17,19,18,

		21,20,22,
		23,21,22,
	};

	// インデックスバッファ生成
	m_IndexBuffer.Create(indices);

	// シェーダオブジェクト生成
	m_Shader.Create("shader/unlitTextureVS.hlsl", "shader/unlitTexturePS.hlsl");
	//	m_Shader.Create("shader/litTextureVS.hlsl", "shader/litTexturePS.hlsl");

	bool sts = m_Texture.Load("assets/texture/NoTexture.png");
	assert(sts == true);

	//モデルによってスケールを調整
	m_Scale.x = 20.0f;
	m_Scale.y = 20.0f;
	m_Scale.z = 20.0f;

	m_Position.x = 80.0f;
	m_Position.y = 200.0f;
	m_Position.z = 0.0f;

	color = { 1.0f,1.0f,1.0f,1.0f };

	rigid.SetMass(2.0f);

	jump = new JumpComponent(100.0f);
}


//=======================================
//更新処理
//=======================================
void Test_Transparent::Update()
{
	bool jumpPress = false;
	if (Input::GetButtonPress(XINPUT_RIGHT) || Input::GetKeyPress(VK_L)) {
		//rigid.ApplyForce(Vector3(5.0f, 0.0f, 0.0f)); // 右に力を加える
	//	m_Position.x += 5.0f;

		rigid.ConstantVelocity_X(80.0f);
	}
	if (Input::GetButtonPress(XINPUT_LEFT) || Input::GetKeyPress(VK_J)) {
		//rigid.ApplyForce(Vector3(-5.0f, 0.0f, 0.0f)); // 左に力を加える
	//	m_Position.x -= 5.0f;
		rigid.ConstantVelocity_X(-80.0f);
	}
	if (Input::GetButtonPress(XINPUT_UP) || Input::GetKeyPress(VK_I)) {
		//rigid.ApplyForce(Vector3(0.0f, 5.0f, 0.0f)); // 上に力を加える
	//	m_Position.y += 5.0f;
		//rigid.ConstantVelocity_Y(60.0f);
		jumpPress = true;
	}
	else {
		jumpPress = false;
	}
	if (Input::GetButtonPress(XINPUT_DOWN) || Input::GetKeyPress(VK_K)) {
		//rigid.ApplyForce(Vector3(0.0f, -5.0f, 0.0f)); // 下に力を加える
	//	m_Position.y -= 5.0f;
		//rigid.ConstantVelocity_Y(-60.0f);
	}
	bool trigger = false;
	if (Input::GetButtonTrigger(XINPUT_UP) || Input::GetKeyTrigger(VK_I)) {
		trigger = true;
	}

	// ジャンプの処理（テスト）
	jump->JumpAction(rigid, jumpPress, isGround,trigger);


	rigid.ReduceVelocity_X();
	//rigid.ReduceVelocity_Y();

	//rigid.GetVelocity();
	rigid.UseGravity(m_Position, true, !jumpPress);
	// リジッドボディの更新
	rigid.AcceleratorPosition(m_Position);

	SetColliderSize_AABB(m_Position, m_Scale);
	SetColliderSize_Sphere(m_Position, 20.0f);

	DirectX::XMVECTOR newRayPos = { m_Position.x,m_Position.y - 10.0f,m_Position.z,0.0f };
	ray.SetOriginPosition(newRayPos);
	ray.SetDirection(DirectX::XMVECTOR{ 0.0f, -1.0f, 0.0f, 0.0f });

	std::vector<TestBoard*>board = Game::GetInstance()->GetObjects<TestBoard>();
	std::vector<TestCube*>cube = Game::GetInstance()->GetObjects<TestCube>();

	isGround = false; // 地面にいるかどうかのフラグを初期化

	DirectX::XMFLOAT3 hitNormal = {};
	// 多くの当たり判定を別々で取って押し戻しができるようにする
	// 当たったら赤く染める
	if (Collision::CheckHit_CubeAndCube_NoTrigger2D_Normal(board[0]->GetColliderSize_AABB(), GetColliderSize_AABB(), m_Position, hitNormal)) {
		
		if (hitNormal.y < -0.5f) {	// 天井
			color.y = 0.0f;
			color.z = 0.0f;
			rigid.UseGravity(m_Position, false, jumpPress);
			isGround = true;
		}
		else if (hitNormal.y > 0.5f) {	// 地面
			color.z = 0.0f;
//			rigid.UseGravity(m_Position, true);
		}
		else if (abs(hitNormal.x) > 0.5f) { // 左右の壁
			color.y = 0.0f;
		//	rigid.UseGravity(m_Position, true);
		}
	}
	else {
	//	rigid.UseGravity(m_Position, true);

		color = { 1.0f,1.0f,1.0f,0.5f };
	}

	if (Collision::CheckHit_CubeAndCube_NoTrigger2D_Normal(cube[0]->GetColliderSize_AABB(), GetColliderSize_AABB(), m_Position, hitNormal)) {
		if (hitNormal.y < -0.5f) {	// 天井
			color.y = 0.0f;
			color.z = 0.0f;
			rigid.UseGravity(m_Position, false, jumpPress);
			isGround = true;
		}
		else if (hitNormal.y > 0.5f) {	// 地面
			color.z = 0.0f;
			//			rigid.UseGravity(m_Position, true);
		}
		else if (abs(hitNormal.x) > 0.5f) { // 左右の壁
			color.y = 0.0f;
			//	rigid.UseGravity(m_Position, true);
		}
	}
	else {
		//	rigid.UseGravity(m_Position, true);

		color = { 1.0f,1.0f,1.0f,0.5f };
	}


	/*float t = 0.0f;
	if (Collision::IntersectRayAABB(ray.GetOriginPosition(), ray.GetDirection(), board[0]->GetColliderSize_AABB(), t) ||
		Collision::IntersectRayAABB(ray.GetOriginPosition(), ray.GetDirection(), cube[0]->GetColliderSize_AABB(), t)) {
		color.y = 0.0f;
		color.z = 0.0f;
	}
	else {
		color = { 1.0f,1.0f,1.0f,0.5f };
	}*/
	

	// 当たったら赤く染める
	/*if (Collision::CheckHit_SphereAndSphere_NoTrigger2D(board[0]->GetColliderSize_Sphere(), GetColliderSize_Sphere(), m_Position)) {
		color.y = 0.0f;
		color.z = 0.0f;
	}
	else {
		color = { 1.0f,1.0f,1.0f,1.0f };
	}*/

	// 当たったら以下略
	/*if (Collision::CheckHit_SphereAndCube_NoTrigger2D(GetColliderSize_Sphere(), board[0]->GetColliderSize_AABB(), m_Position)) {
		color.y = 0.0f;
		color.z = 0.0f;
	}
	else {
		color = { 1.0f,1.0f,1.0f,1.0f };
	}*/
}

//=======================================
//描画処理
//=======================================
void Test_Transparent::Draw()
{
	// ラジアンに変換
	float PitchRadians = DirectX::XMConvertToRadians(m_Rotation.x); // X軸回転
	float YawRadians = DirectX::XMConvertToRadians(m_Rotation.y);     // Y軸回転
	float RollRadians = DirectX::XMConvertToRadians(m_Rotation.z);   // Z軸回転

	//定数バッファを更新
	ConstBuffer cb;

	// ワールド変換行列の作成
	// →オブジェクトの位置・大きさ・向きを指定
	cb.matrixWorld = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	cb.matrixWorld *= DirectX::XMMatrixRotationRollPitchYaw(PitchRadians, YawRadians, RollRadians);
	cb.matrixWorld *= DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	cb.matrixWorld = DirectX::XMMatrixTranspose(cb.matrixWorld);

	cb.color = DirectX::XMFLOAT4(color);

	// 描画の処理
	// トポロジーをセット（プリミティブタイプ）
	g_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 頂点の結び方の規則
	m_Shader.SetGPU();
	m_VertexBuffer.SetGPU();
	m_IndexBuffer.SetGPU();
	m_Texture.SetGPU();

	m_Camera->SetCamera(0);

	// 行列をシェーダーに渡す
	g_pDeviceContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb, 0, 0);

	g_pDeviceContext->DrawIndexed(
		36,							// 描画するインデックス数（立方体なんで36）
		0,							// 最初のインデックスバッファの位置
		0);
}

//=======================================
//終了処理
//=======================================
void Test_Transparent::Uninit()
{
	delete jump;
	jump = nullptr;
}