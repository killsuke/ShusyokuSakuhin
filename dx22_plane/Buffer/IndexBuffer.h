#pragma once

#include	<vector>
#include	<wrl/client.h>
#include	"Helper/dx11helper.h"
#include	"System/DirectXRender.h"

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
		const bool sts = CreateIndexBuffer(
			device,										// デバイス
			m_IndexSize,								// インデックス数
			(void*)indices.data(),						// インデックスデータ先頭アドレス
			&m_IndexBuffer);							// インデックスバッファ

		assert(sts == true);
	}

	void SetGPU() {
		// デバイスコンテキスト取得
		ID3D11DeviceContext* devicecontext = DirectXRender::GetDeviceContext();

		// インデックスバッファをセット
		devicecontext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	}

	void SetGPU(ID3D11DeviceContext* context) {

		// インデックスバッファをセット
		context->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	}


	// インデックスバッファを書き換える
	void Modify(const std::vector<unsigned int>& vertices)
	{
		ID3D11DeviceContext* deviceContext = DirectXRender::GetDeviceContext();

		//頂点データ書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		HRESULT hr = deviceContext->Map(
			m_IndexBuffer.Get(),
			0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);

		if (SUCCEEDED(hr)) {
			memcpy(msr.pData, vertices.data(), vertices.size() * sizeof(unsigned int));
			deviceContext->Unmap(m_IndexBuffer.Get(), 0);
		}
	}

	void BufferReset() {
		m_IndexBuffer = nullptr;
	}

	unsigned int GetIndexSize() const { return m_IndexSize; };

	ID3D11Buffer* GetBuffer() {
		return m_IndexBuffer.Get();
	}

	// サイズ取得
	size_t size() const {
		return m_IndexSize;
	}
};
