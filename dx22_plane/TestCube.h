#pragma once

#include "GameObject.h"

//-----------------------------------------------------------------------------
//TestPlaneクラス
//-----------------------------------------------------------------------------
class TestCube:public GameObject {
	// SRT情報（姿勢情報）
	//DirectX::SimpleMath::Vector3 m_Position = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	//DirectX::SimpleMath::Vector3 m_Rotation = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	//DirectX::SimpleMath::Vector3 m_Scale = DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f);

	//// 描画の為の情報（メッシュに関わる情報）
	//IndexBuffer	 m_IndexBuffer; // インデックスバッファ
	//VertexBuffer<VERTEX_3D>	m_VertexBuffer; // 頂点バッファ

	// 描画の為の情報（見た目に関わる部分）
	//Shader m_Shader; // シェーダー
	//Texture m_Texture;	// テクスチャ

public:
	TestCube();
	~TestCube();

	void Init();
	void Draw();
	void Update();
	void Uninit();

	DirectX::SimpleMath::Vector3 GetPos() { return m_Position; };
};