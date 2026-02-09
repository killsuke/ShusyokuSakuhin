#pragma once
#include	<vector>
#include	<wrl/client.h>
#include	"Helper/dx11helper.h"
#include	"System/DirectXRender.h"

using Microsoft::WRL::ComPtr;

//-----------------------------------------------------------------------------
//VertexBufferクラス
//-----------------------------------------------------------------------------
template <typename T>
class VertexBuffer{

	ComPtr<ID3D11Buffer> m_VertexBuffer;

public:
	void Create(const std::vector<T>& vertices) {

		// デバイス取得
		ID3D11Device* device = nullptr;
		device = DirectXRender::GetDevice();
		assert(device);

		// 頂点バッファ作成
		const bool sts = CreateVertexBufferWrite(
			device,
			sizeof(T),						// １頂点当たりバイト数
			(unsigned int)vertices.size(),	// 頂点数
			(void*)vertices.data(),			// 頂点データ格納メモリ先頭アドレス
			&m_VertexBuffer);				// 頂点バッファ

		assert(sts == true);
	}

	// GPUにセット
	void SetGPU() {

		// デバイスコンテキスト取得
		ID3D11DeviceContext* devicecontext = DirectXRender::GetDeviceContext();

		// 頂点バッファをセットする
		const unsigned int stride = sizeof(T);
		const unsigned  offset = 0;
		devicecontext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);

	}

	void SetGPU(ID3D11DeviceContext* context) {

		// 頂点バッファをセットする
		const unsigned int stride = sizeof(T);
		const unsigned  offset = 0;
		context->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
	}

	// 頂点バッファを書き換える
	void Modify(const std::vector<T>& vertices)
	{
		ID3D11DeviceContext* deviceContext = DirectXRender::GetDeviceContext();

		//頂点データ書き換え
		D3D11_MAPPED_SUBRESOURCE msr;
		const HRESULT hr = deviceContext->Map(
			m_VertexBuffer.Get(), 
			0,
			D3D11_MAP_WRITE_DISCARD, 0, &msr);

		if (SUCCEEDED(hr)) {
			memcpy(msr.pData, vertices.data(), vertices.size() * sizeof(T));
			deviceContext->Unmap(m_VertexBuffer.Get(), 0);
		}
	}

	void BufferReset() {
		m_VertexBuffer = nullptr;
	}

	ID3D11Buffer* GetBuffer() const {
		return m_VertexBuffer.Get();
	}

	// サイズ取得
	size_t GetSize() const {
		D3D11_BUFFER_DESC desc;
		m_VertexBuffer->GetDesc(&desc);
		return desc.ByteWidth;
	}
};
