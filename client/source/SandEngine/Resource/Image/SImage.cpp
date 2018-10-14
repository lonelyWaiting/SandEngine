#include "SandEnginePCH.h"
#include "SImage.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stbimage/stb_image_write.h"

SImageDecode::~SImageDecode()
{
	Clean();
}

void SImageDecode::Clean()
{
	if (m_pData)
	{
		stbi_image_free(m_pData);
		m_pData = nullptr;
	}
}

void SImageDecode::Load(const char* path)
{
	Clean();

	int channelNum = 0;
	m_pData = stbi_loadf(path, &m_width, &m_height, &channelNum, 4);
}

SVector4f SImageDecode::GetPixel(int x, int y)
{
	if (!m_pData)	return SVector4f::Zero();
	int offset = (y * m_width + x) * 4;
	return SVector4f(m_pData[offset], m_pData[offset + 1], m_pData[offset + 2], m_pData[offset + 3]);
}

SImageEncode::SImageEncode(int width, int height)
{
	if (m_pData)	SAFE_DELETE_ARRAY(m_pData);

	m_width  = width;
	m_height = height;
	m_pData  = new float[width * height * 4];
}

SImageEncode::~SImageEncode()
{
	if (m_pData)	SAFE_DELETE_ARRAY(m_pData);
}

void SImageEncode::SetPixel(int x, int y, float r, float g, float b, float a)
{
	if (x < 0 || x >= m_width || y < 0 || y >= m_height)	return;

	int offset = (y * m_width + x) * 4;
	m_pData[offset]     = r;
	m_pData[offset + 1] = g;
	m_pData[offset + 2] = b;
	m_pData[offset + 3] = a;
}

void SImageEncode::SetPixel(int x, int y, const SVector3f& c)
{
	SetPixel(x, y, c.x, c.y, c.z, 1.0f);
}

void SImageEncode::SetPixel(int x, int y, const SVector4f& c)
{
	SetPixel(x, y, c.x, c.y, c.z, c.w);
}

void SImageEncode::write(const char* path)
{
	stbi_write_hdr(path, m_width, m_height, 4, m_pData);
}