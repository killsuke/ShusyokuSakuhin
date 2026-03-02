#pragma once

#include	<d3d11.h>
#include	<string>
#include	<wrl/client.h> // ComPtrの定義を含むヘッダファイル
#include	<filesystem>
#include	<DirectXMath.h>
#include	<memory>

//-----------------------------------------------------------------------------
//Textureクラス
//-----------------------------------------------------------------------------
class Texture final
{
private:

	std::string m_Texname = ""; // ファイル名
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_Srv{}; // シェーダーリソースビュー
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SrvMask{};	// マスク用シェーダーリソースビュー

	int m_Width = 0; // 幅
	int m_Height = 0; // 高さ
	int m_Bpp = 0; // BPP

	// UV座標の情報
	float m_NumU = 1.0f;
	float m_NumV = 1.0f;
	float m_SplitX = 1.0f;
	float m_SplitY = 1.0f;

public:
	Texture() = default;
	~Texture() = default;

	Texture(const Texture& other);

	std::unique_ptr<Texture> Clone() const;

	bool Load(const std::string& filename);
	bool LoadMask(const std::string& filename);
	bool LoadTexture(const std::string& filename);

	bool LoadFromFemory(const unsigned char* data,const int len);

	void SetGPU();
	void SetGPU(ID3D11DeviceContext* context);
	void SetGPU_Mask();

	DirectX::XMMATRIX MakeUV(const float u,const float v,const float uw,const float vh);

	void SetUV(const float nu,const float nv,const float sx,const float sy) {
		m_NumU = nu;
		m_NumV = nv;
		m_SplitX = sx;
		m_SplitY = sy;
	};

	inline DirectX::XMFLOAT4 GetUVSets() {
		DirectX::XMFLOAT4 uvs;

		uvs.x = m_NumU;
		uvs.y = m_NumV;
		uvs.z = m_SplitX;
		uvs.w = m_SplitY;

		return uvs;
	};

	std::string GetTexname() const { return m_Texname; };

	inline void SetInitialCut(const float sx, const float sy) {
		m_SplitX = sx;
		m_SplitY = sy;
	};

	inline void SetCutNum(const float nu, const float nv) {
		m_NumU = nu;
		m_NumV = nv;
	};
};