#pragma once

#include "SandEngine/SApiDef.h"
#include "SandBase/Math/SVector4f.h"

class SAND_API SImageDecode
{
public:
	~SImageDecode();

public:
	void Load(const char* path);
	void Clean();

public:
	int			GetWidth() { return m_width; }
	int			GetHeight() { return m_height; }
	SVector4f	GetPixel(int x, int y);

private:
	float* m_pData  = nullptr;
	int    m_width  = 0;
	int    m_height = 0;
};

class SAND_API SImageEncode
{
public:
	SImageEncode(int width, int height);
	~SImageEncode();

public:
	void SetPixel(int x, int y, float r, float g, float b, float a);
	void SetPixel(int x, int y, const SVector3f& c);
	void SetPixel(int x, int y, const SVector4f& c);

	void write(const char* path);

private:
	float* m_pData  = nullptr;
	int    m_width  = 0;
	int    m_height = 0;
};