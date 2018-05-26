#pragma once

#include "RenderState.h"
#include "SandBase/String/SString.h"

class SMatObj
{
public:
	SMatObj( const char* );

public:
	void SetDoubleSurface( bool doubleFace );
	void SetWireFrameState( bool enable );
	void EnsureLoaded();

public:
	eCullMode	m_cullMode   = eCM_None;
	bool		m_wireframe  = false;
	SString		m_MatName;

private:
	BlendState        m_blend;
	RasterizerState   m_rasterizer;
	DepthStencilState m_depthStencil;
};