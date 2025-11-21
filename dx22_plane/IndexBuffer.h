#pragma once

#include	<vector>
#include	<wrl/client.h>
#include	"dx11helper.h"
#include	"DirectXRender.h"

using Microsoft::WRL::ComPtr;

//-----------------------------------------------------------------------------
//IndexBufferクラス
//-----------------------------------------------------------------------------
class IndexBuffer {
private:
	ComPtr<ID3D11Buffer> m_IndexBuffer;
	unsigned int m_IndexSize = 0;

public:
	void Create(const std::vector<unsigned int>& indices) {

		m_IndexSize = static_cast<unsigned int>(indices.size());

		// デバイス取得
		ID3D11Device* device = nullptr;

		device = DirectXRender::GetDevice();

		assert(device);

		// インデックスバッファ作成
		bool sts = CreateIndexBuffer(
			device,										// デバイス
			m_IndexSize,								// インデックス数
			(void*)indices.data(),						// インデックスデータ先頭アドレス
			&m_IndexBuffer);							// インデックスバッファ

		assert(sts == true);
	}

	void SetGPU() {
		// デバイスコンテキスト取得
		ID3D11DeviceContext* devicecontext = nullptr;
		devicecontext = DirectXRender::GetDeviceContext();

		// インデックスバッファをセット
		devicecontext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	}

	unsigned int GetIndexSize() const { return m_IndexSize; };

	ID3D11Buffer* GetBuffer() {
		return m_IndexBuffer.Get();
	}
};
