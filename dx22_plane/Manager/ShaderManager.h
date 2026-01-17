#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include "dx11helper.h"

struct ShaderKey {
	std::string file = "";
	std::string entry = "";
	std::string model = "";

	bool operator==(const ShaderKey& other) const {
		return file == other.file && entry == other.entry && model == other.model;
	}
};

struct ShaderKeyHash {
	std::size_t operator()(const ShaderKey& k) const {
		return std::hash<std::string>()(k.file) ^ std::hash<std::string>()(k.entry) ^ std::hash<std::string>()(k.model);
	}
};

class ShaderManager final
{
private:
	static std::unordered_map<ShaderKey, std::vector<unsigned char>, ShaderKeyHash> m_ShaderCache;

	// コンストラクタ・デストラクタを削除
	ShaderManager() = delete;
	~ShaderManager() = delete;

	// コピー・ムーブも削除
	ShaderManager(const ShaderManager&) = delete;
	ShaderManager(ShaderManager&&) = delete;
	ShaderManager& operator=(const ShaderManager&) = delete;
	ShaderManager& operator=(ShaderManager&&) = delete;

	static std::string ReplaceExt(const std::string& path, const std::string& newExt);
	static bool FileExists(const std::string& path);
	static bool ReadCsoFile(const std::string& path, std::vector<unsigned char>& out);
	static bool SaveCsoFile(const std::string& path, const void* data, const size_t& size);
	static std::vector<std::string> GetShaderFiles(const std::string& folder);

public:
	static void Init();
	static void UnInit();
	static HRESULT CreateShader(const char* file,const LPCSTR& entry,const LPCSTR& model,void** shaderObj,size_t& size, ID3DBlob** blob);
};