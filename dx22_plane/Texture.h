#pragma once

#include	<d3d11.h>
#include	<string>
#include	<wrl/client.h> // ComPtrの定義を含むヘッダファイル
#include	<filesystem>
#include	<SimpleMath.h>

using Microsoft::WRL::ComPtr;

//-----------------------------------------------------------------------------
//Textureクラス
//-----------------------------------------------------------------------------
class Texture
{
private:

	std::string m_texname{}; // ファイル名
	ComPtr<ID3D11ShaderResourceView> m_srv{}; // シェーダーリソースビュー
	ComPtr<ID3D11ShaderResourceView> m_srvMask{};	// マスク用シェーダーリソースビュー

	int m_width = 0; // 幅
	int m_height = 0; // 高さ
	int m_bpp = 0; // BPP

	// UV座標の情報
	float m_NumU = 1;
	float m_NumV = 1;
	float m_SplitX = 1;
	float m_SplitY = 1;

public:
	bool Load(const std::string& filename);
	bool LoadMask(const std::string& filename);

	bool LoadFromFemory(const unsigned char* data,int len);

	void SetGPU();
	void SetGPU_Mask();

	DirectX::SimpleMath::Matrix MakeUV(float u, float v, float uw, float vh);

	void SetUV(float nu,float nv,float sx,float sy) {
		m_NumU = nu;
		m_NumV = nv;
		m_SplitX = sx;
		m_SplitY = sy;
	};

	inline DirectX::SimpleMath::Vector4 GetUVSets() {
		DirectX::SimpleMath::Vector4 uvs;

		uvs.x = m_NumU;
		uvs.y = m_NumV;
		uvs.z = m_SplitX;
		uvs.w = m_SplitY;

		return uvs;
	};

	inline void SetInitialCut(float sx, float sy) {
		m_SplitX = sx;
		m_SplitY = sy;
	};

	inline void SetCutNum(float nu, float nv) {
		m_NumU = nu;
		m_NumV = nv;
	};
};