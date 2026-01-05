#include	<iostream>
#include	"Texture.h"
#include	"stb_image.h"
#include	"System/DirectXRender.h"
#include	"Manager/TextureManager.h"

using namespace DirectX::SimpleMath;

Texture::Texture(const Texture& other)
	: m_Texname(other.m_Texname),
	m_srv(other.m_srv),           // ComPtr は参照カウントが増えるだけ
	m_srvMask(other.m_srvMask),
	m_width(other.m_width),
	m_height(other.m_height),
	m_bpp(other.m_bpp),
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

	std::string Isfilename = filename;

	// 画像読み込み
	pixels = stbi_load(Isfilename.c_str(), &m_width, &m_height, &m_bpp, 4);
	if (pixels == nullptr) {
		std::cout << Isfilename.c_str() << " Load error " << std::endl;
		return false;
	}

	// テクスチャ2Dリソース生成
	ComPtr<ID3D11Texture2D> pTexture;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Width = m_width;
	desc.Height = m_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			// RGBA
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA subResource{};
	subResource.pSysMem = pixels;
	subResource.SysMemPitch = desc.Width * 4;			// RGBA = 4 bytes per pixel
	subResource.SysMemSlicePitch = 0;

	ID3D11Device* device = DirectXRender::GetDevice();

	HRESULT hr = device->CreateTexture2D(&desc, &subResource, pTexture.GetAddressOf());
	if (FAILED(hr)) {
		stbi_image_free(pixels);
		return false;
	}

	// SRV生成
	hr = device->CreateShaderResourceView(pTexture.Get(), nullptr, m_srv.GetAddressOf());
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

	std::string Isfilename = filename;

	// 画像読み込み
	pixels = stbi_load(Isfilename.c_str(), &m_width, &m_height, &m_bpp, 4);
	if (pixels == nullptr) {
		std::cout << Isfilename.c_str() << " Load error " << std::endl;
		return false;
	}

	// テクスチャ2Dリソース生成
	ComPtr<ID3D11Texture2D> pTexture;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Width = m_width;
	desc.Height = m_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			// RGBA
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA subResource{};
	subResource.pSysMem = pixels;
	subResource.SysMemPitch = desc.Width * 4;			// RGBA = 4 bytes per pixel
	subResource.SysMemSlicePitch = 0;

	ID3D11Device* device = DirectXRender::GetDevice();

	HRESULT hr = device->CreateTexture2D(&desc, &subResource, pTexture.GetAddressOf());
	if (FAILED(hr)) {
		stbi_image_free(pixels);
		return false;
	}

	// SRV生成
	hr = device->CreateShaderResourceView(pTexture.Get(), nullptr, m_srvMask.GetAddressOf());
	if (FAILED(hr)) {
		stbi_image_free(pixels);
		return false;
	}

	// ピクセルイメージ解放
	stbi_image_free(pixels);

	return true;
}

// テクスチャをメモリからロード
bool Texture::LoadFromFemory(const unsigned char* Data,int len) {

	bool sts = true;
	unsigned char* pixels;

	// 画像読み込み
	pixels = stbi_load_from_memory(Data, 
		len, 
		&m_width, 
		&m_height, 
		&m_bpp, 
		STBI_rgb_alpha);

	// テクスチャ2Dリソース生成
	ComPtr<ID3D11Texture2D> pTexture;

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	desc.Width = m_width;
	desc.Height = m_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			// RGBA
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA subResource{};
	subResource.pSysMem = pixels;
	subResource.SysMemPitch = desc.Width * 4;			// RGBA = 4 bytes per pixel
	subResource.SysMemSlicePitch = 0;

	ID3D11Device* device = DirectXRender::GetDevice();

	HRESULT hr = device->CreateTexture2D(&desc, &subResource, pTexture.GetAddressOf());
	if (FAILED(hr)) {
		stbi_image_free(pixels);
		return false;
	}

	// SRV生成
	hr = device->CreateShaderResourceView(pTexture.Get(), nullptr, m_srv.GetAddressOf());
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
	auto tex = TextureManager::LoadTexture(filename);

	if (tex != nullptr) {
		m_Texname = filename;
		m_srv = tex;
		ID3D11DeviceContext* devicecontext = DirectXRender::GetDeviceContext();
		devicecontext->PSSetShaderResources(0, 1, m_srv.GetAddressOf());
	}
	else {
		MessageBoxA(NULL, "テクスチャをシェーダーリソースビューにセット出来ませんでした。", "エラー", MB_ICONERROR | MB_OK);
		return false;
	}

	return true;
}

// テクスチャをGPUにセット
void Texture::SetGPU()
{
	if (m_srv.Get() == nullptr) {
		return;
	}
	ID3D11DeviceContext* devicecontext = DirectXRender::GetDeviceContext();
	
	devicecontext->PSSetShaderResources(0, 1, m_srv.GetAddressOf());
}

void Texture::SetGPU_Mask()
{
	ID3D11DeviceContext* devicecontext = DirectXRender::GetDeviceContext();
	devicecontext->PSSetShaderResources(1, 1, m_srvMask.GetAddressOf());
}

Matrix Texture::MakeUV(float u, float v, float uw, float vh) {
	// ＵＶの行列作成
	Matrix mat = Matrix::CreateScale(uw, vh, 1.0f);
	mat *= Matrix::CreateTranslation(u, v, 0.0f).Transpose();

	return mat;
}