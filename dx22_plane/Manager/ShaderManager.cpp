#include "ShaderManager.h"

std::unordered_map<ShaderKey, std::vector<unsigned char>, ShaderKeyHash> ShaderManager::m_ShaderCache;

void ShaderManager::Init() {
	m_ShaderCache.clear();
}

void ShaderManager::UnInit() {
	m_ShaderCache.clear();
}

HRESULT ShaderManager::CreateShader(const char* file,const LPCSTR& entry,const LPCSTR& model, void** shaderObj, size_t& size, ID3DBlob** blob) {

	ShaderKey key{ file, entry, model };

	// １．キャッシュに存在するか確認
	std::unordered_map<ShaderKey, std::vector<unsigned char>, ShaderKeyHash>::iterator it =
		m_ShaderCache.find(key);

	if (it != m_ShaderCache.end()) {
		*shaderObj = it->second.data();
		size = it->second.size();
		return S_OK;
	}

	// ２．csoが存在するか確認
	std::string csoPath = ReplaceExt(file, "cso");
	if (FileExists(csoPath)) {
		std::vector<unsigned char> data;
		ReadCsoFile(csoPath, data);

		m_ShaderCache[key] = data;
		*shaderObj = m_ShaderCache[key].data();
		size = m_ShaderCache[key].size();
		return S_OK;
	}

	HRESULT hr = S_OK;

	// ３．コンパイルしてキャッシュに保存
	hr = CompileShader(file, entry, model, shaderObj, size, blob);
	if(FAILED(hr)) {
		return hr;
	}

	// ４．csoを保存
	SaveCsoFile(csoPath, (*blob)->GetBufferPointer(), (*blob)->GetBufferSize());

	// ５．キャッシュに保存
	m_ShaderCache[key] = std::vector<unsigned char>(
		reinterpret_cast<unsigned char*>((*blob)->GetBufferPointer()),
		reinterpret_cast<unsigned char*>((*blob)->GetBufferPointer()) + (*blob)->GetBufferSize()
	);

	*shaderObj = m_ShaderCache[key].data();
	size = m_ShaderCache[key].size();

	return S_OK;
}

// 拡張子を置き換える
std::string ShaderManager::ReplaceExt(const std::string& path, const std::string& newExt) {

	size_t pos = path.find_last_of('.');	// 最後に出てくるドットの位置を探す
	if (pos == std::string::npos) {	// ドットが見つからなかった場合、std::string::npos = 見つからなかったことを示す特殊な値
		return path + "." + newExt;
	}

	return path.substr(0, pos + 1) + newExt;	// ドットまでの部分文字列と新しい拡張子を結合して返す
}

// このパスが存在するか確認
bool ShaderManager::FileExists(const std::string& path) {

	return std::filesystem::exists(path);
}

// csoファイルの中身を読み取る
bool ShaderManager::ReadCsoFile(const std::string& path, std::vector<unsigned char>& out) {

	std::ifstream file(path, std::ios::binary);	// バイナリモードでファイルを開く
	if(!file) {
		return false;
	}

	file.seekg(0, std::ios::end);	// ファイルの終端に移動
	const size_t size = file.tellg();		// ファイルサイズを取得（ファイルの末端にいるので、現在位置がサイズになる）
	file.seekg(0, std::ios::beg);	// ファイルの先頭に戻る

	out.resize(size);	// バッファのサイズを調整
	file.read(reinterpret_cast<char*>(out.data()), size);	// ファイルの内容を読み取る

	return true;
}

// csoファイルを保存する
bool ShaderManager::SaveCsoFile(const std::string& path, const void* data,const size_t& size) {

	std::ofstream file(path, std::ios::binary);	// バイナリモードでファイルを開く
	if(!file) {
		return false;
	}

	file.write(reinterpret_cast<const char*>(data), size);	// データを書き込む
	return true;
}