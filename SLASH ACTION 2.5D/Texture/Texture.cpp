#include	<iostream>
#include	"Texture/Texture.h"
#include	"Stb_Image/stb_image.h"
#include	"System/DirectXRender.h"
#include	"Manager/TextureManager.h"

using namespace DirectX;
using Microsoft::WRL::ComPtr;

namespace {
	constexpr int BYTE4 = 4;
}

Texture::Texture(const Texture& other)
	: m_Texname(other.m_Texname),
	m_Srv(other.m_Srv),           // ComPtr は参照カウントが増えるだけ
	m_SrvMask(other.m_SrvMask),
	m_Width(other.m_Width),
	m_Height(other.m_Height),
	m_Bpp(other.m_Bpp),
	m_NumU(other.m_NumU),
	m_NumV(other.m_NumV),
	m_SplitX(other.m_SplitX),
	m_SplitY(other.m_SplitY)
{
	// 追加の初期化が必要ならここに
}

std::unique_ptr<Texture> Texture::Clone() const {

	return std::make_unique<Texture>(*this);
}

// テクスチャをロード
bool Texture::Load(const std::string& filename)
{
	bool sts = true;
	unsigned char* pixels;

	const std::string Isfilename = filename;

	// 画像読み込み
	pixels = stbi_load(Isfilename.c_str(), &m_Width, &m_Height, &m_Bpp, BYTE4);
	if (pixels == nullptr) {
		std::cout << Isfilename.c_str() << " Load error " << std::endl;
		return false;
	}

	// テクスチャ2Dリソース生成
	ComPtr<ID3D11Texture2D> pTexture;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Width = m_Width;
	desc.Height = m_Height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			// RGBA
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA subResource{};
	subResource.pSysMem = pixels;
	subResource.SysMemPitch = desc.Width * BYTE4;			// RGBA = 4 bytes per pixel
	subResource.SysMemSlicePitch = 0;

	ID3D11Device* device = DirectXRender::GetDevice();

	HRESULT hr = device->CreateTexture2D(&desc, &subResource, pTexture.GetAddressOf());
	if (FAILED(hr)) {
		stbi_image_free(pixels);
		return false;
	}

	// SRV生成
	hr = device->CreateShaderResourceView(pTexture.Get(), nullptr, m_Srv.GetAddressOf());
	if (FAILED(hr)) {
		stbi_image_free(pixels);
		return false;
	}

	// ピクセルイメージ解放
	stbi_image_free(pixels);

	return true;
}

bool Texture::LoadMask(const std::string& filename)
{
	bool sts = true;
	unsigned char* pixels;

	const std::string Isfilename = filename;

	// 画像読み込み
	pixels = stbi_load(Isfilename.c_str(), &m_Width, &m_Height, &m_Bpp, BYTE4);
	if (pixels == nullptr) {
		std::cout << Isfilename.c_str() << " Load error " << std::endl;
		return false;
	}

	// テクスチャ2Dリソース生成
	ComPtr<ID3D11Texture2D> pTexture;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Width = m_Width;
	desc.Height = m_Height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			// RGBA
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA subResource{};
	subResource.pSysMem = pixels;
	subResource.SysMemPitch = desc.Width * BYTE4;			// RGBA = 4 bytes per pixel
	subResource.SysMemSlicePitch = 0;

	ID3D11Device* device = DirectXRender::GetDevice();

	HRESULT hr = device->CreateTexture2D(&desc, &subResource, pTexture.GetAddressOf());
	if (FAILED(hr)) {
		stbi_image_free(pixels);
		return false;
	}

	// SRV生成
	hr = device->CreateShaderResourceView(pTexture.Get(), nullptr, m_SrvMask.GetAddressOf());
	if (FAILED(hr)) {
		stbi_image_free(pixels);
		return false;
	}

	// ピクセルイメージ解放
	stbi_image_free(pixels);

	return true;
}

// テクスチャをメモリからロード
bool Texture::LoadFromFemory(const unsigned char* Data,const int len) {

	bool sts = true;
	unsigned char* pixels;

	// 画像読み込み
	pixels = stbi_load_from_memory(Data, 
		len, 
		&m_Width, 
		&m_Height, 
		&m_Bpp, 
		STBI_rgb_alpha);

	// テクスチャ2Dリソース生成
	ComPtr<ID3D11Texture2D> pTexture;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Width = m_Width;
	desc.Height = m_Height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			// RGBA
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA subResource{};
	subResource.pSysMem = pixels;
	subResource.SysMemPitch = desc.Width * BYTE4;			// RGBA = 4 bytes per pixel
	subResource.SysMemSlicePitch = 0;

	ID3D11Device* device = DirectXRender::GetDevice();

	HRESULT hr = device->CreateTexture2D(&desc, &subResource, pTexture.GetAddressOf());
	if (FAILED(hr)) {
		stbi_image_free(pixels);
		return false;
	}

	// SRV生成
	hr = device->CreateShaderResourceView(pTexture.Get(), nullptr, m_Srv.GetAddressOf());
	if (FAILED(hr)) {
		stbi_image_free(pixels);
		return false;
	}

	// ピクセルイメージ解放
	stbi_image_free(pixels);

	return true;
}

bool Texture::LoadTexture(const std::string& filename)
{
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> tex = TextureManager::LoadTexture(filename);

	if (tex != nullptr) {
		m_Texname = filename;
		m_Srv = tex;
		ID3D11DeviceContext* devicecontext = DirectXRender::GetDeviceContext();
		devicecontext->PSSetShaderResources(0, 1, m_Srv.GetAddressOf());
	}
	else {
		MessageBoxW(NULL, L"テクスチャをシェーダーリソースビューにセット出来ませんでした。", L"エラー", MB_ICONERROR | MB_OK);
		return false;
	}

	return true;
}

// テクスチャをGPUにセット
void Texture::SetGPU()
{
	if (m_Srv.Get() == nullptr) {
		return;
	}
	ID3D11DeviceContext* devicecontext = DirectXRender::GetDeviceContext();
	
	devicecontext->PSSetShaderResources(0, 1, m_Srv.GetAddressOf());
}

void Texture::SetGPU(ID3D11DeviceContext* context)
{
	if (m_Srv.Get() == nullptr) {
		return;
	}

	context->PSSetShaderResources(0, 1, m_Srv.GetAddressOf());
}

void Texture::SetGPU_Mask()
{
	ID3D11DeviceContext* devicecontext = DirectXRender::GetDeviceContext();
	devicecontext->PSSetShaderResources(1, 1, m_SrvMask.GetAddressOf());
}

DirectX::XMMATRIX Texture::MakeUV(const float u,const float v,const float uw,const float vh) {
	// ＵＶの行列作成
	const XMMATRIX scaleMtx = XMMatrixScaling(uw, vh, 1.0f);
	const XMMATRIX transMtx = XMMatrixTranslation(u, v, 0.0f);
	const XMMATRIX transMtxTranspose = XMMatrixTranspose(transMtx);

	return scaleMtx * transMtxTranspose;
}