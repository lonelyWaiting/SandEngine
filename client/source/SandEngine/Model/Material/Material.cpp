#include "SandEnginePCH.h"
#include "Material.h"

SMatObj::SMatObj( const char* name )
{
	m_MatName = name;
}

void SMatObj::SetDoubleSurface( bool doubleFace )
{
	m_cullMode = doubleFace ? eCM_None : eCM_Back;
}

void SMatObj::SetWireFrameState( bool enable )
{
	m_wireframe = enable ? eFM_Wireframe : eFM_Solid;
}

void SMatObj::EnsureLoaded()
{
	m_rasterizer.GetDesc().cullMode = m_cullMode;
	m_rasterizer.GetDesc().fillMode = m_wireframe ? eFM_Wireframe : eFM_Solid;
	m_rasterizer.ComputeHash();
	m_blend.ComputeHash();
	m_depthStencil.ComputeHash();
}