#pragma once
#include "BoneData.h"
#include "Render.h"
#include <SimpleMath.h>

class Bones : public RenderComponent
{
private:
	Bone* m_bones = nullptr;				// ボーン
	DirectX::SimpleMath::Matrix* g_combMtx = nullptr;				// 合成変換行列。これがシェーダに渡ります

	std::vector<AnimationVertex> m_boneVertices;
	std::vector<unsigned int>	m_indices;		// インデックスデータ群

	VertexBuffer<AnimationVertex> m_AnimationVertexBuffer = {};

public:
	Bones(GameObject& obj);	// デフォルトコンストラクタは非公開にする
	~Bones();	// デストラクタはデフォルトでOK
	
	void Update();	// ボーンの更新処理

	const std::vector<AnimationVertex>& GetBoneVertices() {
		return m_boneVertices;
	}
	void SetShaderOneSkin(const std::string& vertex, const std::string& pixel) { m_Shader->CreateOneSkinAnimation(vertex, pixel); };
	void SetBoneMesh(Mesh& mesh);
	std::vector<AnimationVertex> CreateBoneMeshVertices();
	std::vector<unsigned int> CreateMeshIndices();

	void DX11MtxRotationX(float angle, DirectX::XMFLOAT4X4& outmtx);
	void DX11MtxRotationY(float angle, DirectX::XMFLOAT4X4& outmtx);
	void DX11MtxRotationZ(float angle, DirectX::XMFLOAT4X4& outmtx);
	void DX11MtxInverse(DirectX::XMFLOAT4X4& ansmtx, const DirectX::XMFLOAT4X4& mtx);
};