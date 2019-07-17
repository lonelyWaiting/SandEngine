#pragma once

#include "fake_unity_shader/src/shader_parser.h"

class SShader
{
public:
	static void Init(struct ID3D11Device* device , const char* shader_parser_search_path = nullptr);
	static void Cleanup();
	static SShader* FindOrAdd(const char* filename);

private:
	SShader(const char* filename);

public:
	int GetPassCount() const { return (int) info.pass_list.size(); }
	const shaderPassInfo& GetPass(int index) const;

private:
	shaderInfo info;
};