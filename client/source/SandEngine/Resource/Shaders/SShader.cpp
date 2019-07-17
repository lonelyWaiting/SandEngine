#include "SShader.h"
#include "fake_unity_shader/src/shader.h"
#include "SandBase/String/SString.h"
#include "SandBase/Log/SLog.h"
#include <map>

class SShaderManager
{
public:
	static SShaderManager& Get()
	{
		static SShaderManager manager;
		return manager;
	}

	void Add(const char* filename, SShader* shader)
	{
		if (filename == nullptr)
		{
			SLog::Error("Shader Filename must be exist");
			return;
		}

		if (shader == nullptr)
		{
			SLog::Error("Shader must exist");
			return;
		}

		shaderMap[filename] = shader;
	}

	SShader* Find(const char* filename)
	{
		if (!filename)	return false;
		auto iter = shaderMap.find(filename);
		return iter != shaderMap.end() ? iter->second : nullptr;
	}

private:
	SShaderManager() {}

private:
	std::map<SString , SShader*> shaderMap;
};

void SShader::Init(ID3D11Device* device , const char* shader_parser_search_path = nullptr)
{
	shader_init(device , shader_parser_search_path);
}

void SShader::Cleanup()
{
	shader_cleanup();
}

SShader* SShader::FindOrAdd(const char* filename)
{
	SShader* shader = SShaderManager::Get().Find(filename);
	if (!shader) shader = new SShader(filename);
	return shader;
}

SShader::SShader(const char* filename)
{
	bool success = shader_load(filename);
	if (success)
	{
		SShaderManager::Get().Add(filename , this);
	}
}

const shaderPassInfo & SShader::GetPass(int index) const
{
	if (index < 0 || index >= info.pass_list.size())
	{
		SLog::Warning("shader pass index invalidate");
		return;
	}

	return info.pass_list[index];
}
