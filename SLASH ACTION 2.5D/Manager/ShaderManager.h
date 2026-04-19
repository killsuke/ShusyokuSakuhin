#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <fstream>
#include "Helper/dx11helper.h"

// シェーダーの種類を表す列挙型
// 必要とあれば他のシェーダータイプも追加可能
enum class ShaderType {
	VS,	// 頂点シェーダー
	PS,	// ピクセルシェーダー
	GS,	// ジオメトリシェーダー

	MAX	// シェーダータイプの最大値
};

struct ShaderBinary {
	std::vector<uint8_t> data;
};

class ShaderManager final
{
private:
	static inline std::unordered_map<std::string, ShaderBinary> m_Binaries;

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
	static std::string ResolveShaderPath(const std::string& file, const ShaderType& type);
	static const char* ShaderTypeToString(const ShaderType& type);
	static std::string MakeKey(const std::string& file, const ShaderType& type);
	static void LoadAllCSO();
	static const ShaderBinary* GetBinary(const std::string& file, const ShaderType& type);
	static const ShaderBinary* GetBinaryByKey(const std::string& key);

public:
	static void Init();
	static void UnInit();
	static HRESULT CreateShader(const char* file, const ShaderType& type, const LPCSTR& entry, void** shaderObj, size_t& size);
	static std::string ToCsoPath(const std::string& file, const ShaderType& type);
	static bool NeedsCompile(const std::string& file, const ShaderType& type);
	static void CompileShaderSet_Dirty();
};