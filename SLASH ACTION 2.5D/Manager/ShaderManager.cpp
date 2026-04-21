#include "ShaderManager.h"
#include <iostream>

using namespace std::filesystem;

namespace {
	std::string FILE_PATH_TO_SHADER = "ShaderResource/"; // シェーダーファイルのパス
	std::string FILE_PATH_TO_SHADER_HLSL = "ShaderResource/hlsl/"; // シェーダーファイルのパス（HLSL）
	std::string FILE_PATH_TO_SHADER_CSO = "ShaderResource/cso/"; // シェーダーファイルのパス（CSO）
}

void ShaderManager::Init() {

	CompileShaderSet_Dirty();
	m_Binaries.clear();
	LoadAllCSO();

	std::cout << "ShaderManager内で初期化完了" << std::endl;
}

void ShaderManager::UnInit() {
	m_Binaries.clear();
}

HRESULT ShaderManager::CreateShader(const char* file, const ShaderType& type, const LPCSTR& entry, void** shaderObj, size_t& size) {

	HRESULT hr = S_OK;

	std::string key = MakeKey(file, type);

	const ShaderBinary* binary = GetBinaryByKey(key); // キーで直接探す関数があると便利
	if (binary) {
		// キャッシュにあればそれを使う
		*shaderObj = (void*)binary->data.data();
		size = binary->data.size();
		return S_OK;
	}

	// 見つからない場合はエラー
	MessageBoxW(NULL, L"シェーダーが見つかりませんでした。", L"エラー", MB_ICONERROR | MB_OK);

	return E_FAIL;
}

// 拡張子を置き換える
std::string ShaderManager::ReplaceExt(const std::string& path, const std::string& newExt) {

	const size_t pos = path.find_last_of('.');	// 最後に出てくるドットの位置を探す
	if (pos == std::string::npos) {	// ドットが見つからなかった場合、std::string::npos = 見つからなかったことを示す特殊な値
		return path + "." + newExt;
	}

	return path.substr(0, pos + 1) + newExt;	// ドットまでの部分文字列と新しい拡張子を結合して返す
}

// このパスが存在するか確認
bool ShaderManager::FileExists(const std::string& path) {

	return exists(path);
}

// csoファイルの中身を読み取る
bool ShaderManager::ReadCsoFile(const std::string& path, std::vector<unsigned char>& out) {

	std::ifstream file(path, std::ios::binary);	// バイナリモードでファイルを開く
	if (!file) {
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
bool ShaderManager::SaveCsoFile(const std::string& path, const void* data, const size_t& size) {

	std::ofstream file(path, std::ios::binary);	// バイナリモードでファイルを開く
	if (!file) {
		return false;
	}

	file.write(reinterpret_cast<const char*>(data), size);	// データを書き込む
	return true;
}

// 指定フォルダ内のシェーダーファイルを取得する
std::vector<std::string> ShaderManager::GetShaderFiles(const std::string& folder) {

	std::vector<std::string> files;

	// フォルダが存在するか確認
	if (!exists(folder)) {
		MessageBoxW(NULL, L"シェーダーフォルダが見つかりませんでした。", L"エラー", MB_ICONERROR | MB_OK);
		return files;
	}

	for (const directory_entry& entry : directory_iterator(folder)) {

		if (entry.path().extension() == ".hlsl") {
			files.push_back(entry.path().string());
		}
	}

	return files;
}

std::string ShaderManager::ResolveShaderPath(const std::string& file, const ShaderType& type) {

	std::string base = FILE_PATH_TO_SHADER_HLSL;
	return base + ShaderTypeToString(type) + "/" + file;
}

// シェーダータイプを文字列に変換する
const char* ShaderManager::ShaderTypeToString(const ShaderType& type) {

	switch (type)
	{
	case ShaderType::VS:
		return "VS";
	case ShaderType::PS:
		return "PS";
	case ShaderType::GS:
		return "GS";
	default:
		break;
	}

	return "";
}

// シェーダーのキーを作成する
std::string ShaderManager::MakeKey(const std::string& file, const ShaderType& type) {
	std::filesystem::path p(file);
	return p.stem().string() + "_" + ShaderTypeToString(type);
}

void ShaderManager::LoadAllCSO() {

	std::vector<ShaderType> types = {
		ShaderType::VS,
		ShaderType::PS,
		ShaderType::GS
	};

	for (const ShaderType& type : types) {

		std::string folder = FILE_PATH_TO_SHADER_CSO + ShaderTypeToString(type) + "/";

		if (!exists(folder)) {
			continue;	// フォルダが存在しない場合はスキップ
		}

		directory_iterator files = std::filesystem::directory_iterator(folder);

		for (const directory_entry& entry : files) {

			if (entry.path().extension() != ".cso") {
				continue;	// csoファイル以外はスキップ
			}

			// ファイル名だけ抜く
			std::string filePath = entry.path().string();
			std::string key = MakeKey(filePath, type);

			std::vector<uint8_t> data;
			ReadCsoFile(entry.path().string(), data);

			m_Binaries[key] = { data };

			std::cout << "Loaded Key: " << key << " from " << entry.path().filename() << std::endl;
		}
	}
}

const ShaderBinary* ShaderManager::GetBinary(const std::string& file, const ShaderType& type) {

	std::string key = MakeKey(file, type);

	std::unordered_map<std::string, ShaderBinary>::iterator it = m_Binaries.find(key);
	if (it != m_Binaries.end()) {
		return &it->second;
	}

	return nullptr;
}

const ShaderBinary* ShaderManager::GetBinaryByKey(const std::string& key) {
	std::unordered_map<std::string, ShaderBinary>::iterator it = m_Binaries.find(key);
	if (it != m_Binaries.end()) {
		return &it->second;
	}
	return nullptr;
}

std::string ShaderManager::ToCsoPath(const std::string& file, const ShaderType& type) {

	std::string base = FILE_PATH_TO_SHADER_CSO;
	std::string name = ReplaceExt(file, "cso");

	return base + ShaderTypeToString(type) + "/" + name;
}

bool ShaderManager::NeedsCompile(const std::string& file, const ShaderType& type) {

	const std::string hlslPath = ResolveShaderPath(file, type);
	const std::string csoPath = ToCsoPath(file, type);

	if (!exists(csoPath)) {
		return true;	// csoファイルが存在しない場合はコンパイルが必要
	}

	const file_time_type hlslTime = last_write_time(hlslPath);	// hlslファイルの更新時間を取得
	const file_time_type csoTime = last_write_time(csoPath);	// csoファイルの更新時間を取得

	return hlslTime >= csoTime;	// hlslの方が新しい場合はコンパイルが必要
}

void ShaderManager::CompileShaderSet_Dirty() {

	std::vector<ShaderType> types = {
		ShaderType::VS,
		ShaderType::PS,
		ShaderType::GS
	};

	for (const ShaderType& type : types) {

		std::string folder = FILE_PATH_TO_SHADER_HLSL + ShaderTypeToString(type) + "/";

		std::vector<std::string> files = GetShaderFiles(folder);

		for (const std::string& file : files) {

			// ファイル名だけ抜く
			std::string fileName = std::filesystem::path(file).filename().string();

			if (!NeedsCompile(fileName, type)) {
				continue;	// コンパイルが不要な場合はスキップ
			}

			std::cout << "Compile Shader : " << fileName << std::endl;

			const std::string hlslPath = ResolveShaderPath(fileName, type);
			const std::string csoPath = ToCsoPath(fileName, type);

			// csoファイルの親ディレクトリを作成
			create_directories(
				path(csoPath).parent_path()
			);

			std::string entry = "";
			std::string model = "";

			switch (type)
			{
			case ShaderType::VS:
				entry = VS_INFORMATION.entryName;
				model = VS_INFORMATION.modelName;
				break;
			case ShaderType::PS:
				entry = PS_INFORMATION.entryName;
				model = PS_INFORMATION.modelName;
				break;
			case ShaderType::GS:
				entry = GS_INFORMATION.entryName;
				model = GS_INFORMATION.modelName;
				break;
			}

			ID3DBlob* blob = nullptr;
			size_t size = 0;

			HRESULT hr = CompileShader(
				hlslPath.c_str(),
				entry.c_str(),
				model.c_str(),
				&blob
			);

			if (FAILED(hr)) {

				const std::string errorMsg = "シェーダーのコンパイルに失敗しました。該当ファイル: " + fileName;
				MessageBoxW(NULL, std::wstring(errorMsg.begin(), errorMsg.end()).c_str(), L"エラー", MB_ICONERROR | MB_OK);
				continue;
			}

			SaveCsoFile(csoPath, blob->GetBufferPointer(), blob->GetBufferSize());
			blob->Release();
		}
	}
}