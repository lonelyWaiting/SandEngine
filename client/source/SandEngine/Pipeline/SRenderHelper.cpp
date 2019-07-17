#include "SandEnginePCH.h"
#include "SRenderHelper.h"
#include "SandEngine/Resource/Material/Material.h"
#include "SandEngine/Application/SRenderer.h"
#include "SandBase/Log/Slog.h"
#include "SandEngine/Resource/Texture/STextureObject.h"
#include "SandEngine/Resource/SMeshBuffer.h"
#include "SandEngine/Resource/Shaders/SShader.h"
#include "SandEngine/Resource/SStaticMeshManager.h"
#include "SandBase/Math/SVector3f.h"
#include "SandEngine/Resource/Shaders/SShader.h"

ID3D11Device*        SRenderHelper::g_Device           = nullptr;
IDXGISwapChain*      SRenderHelper::g_SwapChain        = nullptr;
ID3D11DeviceContext* SRenderHelper::g_ImmediateContext = nullptr;

#define STREAM_VERTEX_POSITION		0
#define STREAM_VERTEX_NORMAL		1
#define STREAM_VERTEX_COLOR			2
#define STREAM_VERTEX_TEXCOORD0		3
#define STREAM_VERTEX_TEXCOORD1		4
#define STREAM_VERTEX_TEXCOORD2		5
#define STREAM_VERTEX_TEXCOORD3		6
#define STREAM_VERTEX_TEXCOORD4		7
#define STREAM_VERTEX_TEXCOORD5		8
#define STREAM_VERTEX_TEXCOORD6		9
#define STREAM_VERTEX_TEXCOORD7		10
#define STREAM_VERTEX_TEXCOORD8		11
#define STREAM_VERTEX_TEXCOORD9		12
#define STREAM_VERTEX_TEXCOORD10	13
#define STREAM_VERTEX_TEXCOORD11	14
#define STREAM_VERTEX_TEXCOORD12	15
#define STREAM_VERTEX_TEXCOORD13	16
#define STREAM_VERTEX_TEXCOORD14	17
#define STREAM_VERTEX_TEXCOORD15	18

static D3D11_INPUT_ELEMENT_DESC g_InputLayout[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
static ID3D11Buffer* g_VertexBuffer[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
static unsigned int g_VertexBufferStride[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = { 0 };
static unsigned int g_VertexBufferOffset[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT] = { 0 };
static int g_InputLayoutCount   = 0;
static int g_VertexBufferCount = 0;
static ID3D11Buffer* g_IndexBuffer = nullptr;
static std::map<size_t, ID3D11InputLayout*> g_InputLayoutManager;

struct cbFrame
{
	SMatrix4f viewMatrix;
	SMatrix4f projMatrix;
	SMatrix4f viewProjMatrix;
	float tanHalfFovX;
	float tanHalfFovY;
	float nearPlane;
	float farPlane;
	float aspectRatio;
	SVector3f cameraPos;
};

static SBuffer* g_FrameCB = nullptr;

// https://stackoverflow.com/questions/19195183/how-to-properly-hash-the-custom-struct
template<class T>
inline void hash_combine(std::size_t& s, const T& v)
{
	std::hash<T> h;
	s ^= h(v) + 0x9e3779b9 + (s << 6) + (s >> 2);
}

ID3D11InputLayout* FindInputLayer(ID3DBlob* blob)
{
	std::size_t h = 0;
	for (int i = 0; i < g_InputLayoutCount; i++)
	{
		auto element = g_InputLayout[i];
		hash_combine(h, element.SemanticName);
		hash_combine(h, element.SemanticIndex);
		hash_combine(h, element.Format);
		hash_combine(h, element.InputSlot);
		hash_combine(h, element.AlignedByteOffset);
		hash_combine(h, element.InputSlotClass);
		hash_combine(h, element.InstanceDataStepRate);
	}

	auto iter = g_InputLayoutManager.find(h);
	if (iter != g_InputLayoutManager.end())	return iter->second;

	ID3D11InputLayout* inputLayout = nullptr;
	SRenderHelper::g_Device->CreateInputLayout(g_InputLayout, g_InputLayoutCount, blob->GetBufferPointer(), blob->GetBufferSize(), &inputLayout);

	g_InputLayoutManager[h] = inputLayout;

	return inputLayout;
}

void SRenderHelper::Init()
{
	if (g_FrameCB == nullptr)
	{
		g_FrameCB = new SBuffer(eBU_Dynamic, sizeof(cbFrame), 1, nullptr, eBF_Constant );
	}

	SShader::Init(g_Device, );
}

void SRenderHelper::BeginNewFrame()
{
	Camera& camera = SRenderer::Get().GetMainCamera();
	D3D11_MAPPED_SUBRESOURCE data;
	SRenderHelper::g_ImmediateContext->Map(g_FrameCB->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
	cbFrame& cb = *(cbFrame*)(data.pData);
	cb.viewMatrix     = camera.GetViewMatrix();
	cb.projMatrix     = camera.GetProjectMatrix();
	cb.viewProjMatrix = cb.viewMatrix * cb.projMatrix;
	cb.tanHalfFovX    = camera.GetTanHalfFovX();
	cb.tanHalfFovY    = camera.GetTanHalfFovY();
	cb.nearPlane      = camera.GetNearPlane();
	cb.farPlane       = camera.GetFarPlane();
	cb.aspectRatio    = camera.GetAspectRatio();
	cb.cameraPos      = camera.GetPostion();
	cb.viewMatrix.MakeTranspose();
	cb.projMatrix.MakeTranspose();
	cb.viewProjMatrix.MakeTranspose();
	SRenderHelper::g_ImmediateContext->Unmap(g_FrameCB->GetBuffer(), 0);

	ID3D11Buffer* cBuffer = g_FrameCB->GetBuffer();
	SRenderHelper::g_ImmediateContext->VSSetConstantBuffers(0, 1, &cBuffer);
	SRenderHelper::g_ImmediateContext->PSSetConstantBuffers(0, 1, &cBuffer);
}

void SRenderHelper::SetRenderState()
{

}

void ResetStream()
{
	g_InputLayoutCount = g_VertexBufferCount = 0;
}

void AddMeshStream( const SMeshBuffer& mesh , suInt32 mask )
{
	if (!mesh.GetVertexBuffer())	return;

	ID3D11Buffer* pBuffer = mesh.GetVertexBuffer()->GetBuffer();
	if (!pBuffer)	return;

	g_VertexBuffer[g_VertexBufferCount]       = pBuffer;
	g_VertexBufferStride[g_VertexBufferCount] = mesh.GetVertexBuffer()->GetStride();
	g_VertexBufferOffset[g_VertexBufferCount] = 0;

	if (mask & eVA_IndexBuffer)
	{
		auto ib = mesh.GetIndexBuffer();
		if(ib)	g_IndexBuffer = ib->GetBuffer();
	}

	if (mask & eVA_POSITION)
	{
		g_InputLayout[g_InputLayoutCount].SemanticName         = "POSITION";
		g_InputLayout[g_InputLayoutCount].SemanticIndex        = 0;
		g_InputLayout[g_InputLayoutCount].Format               = ConvertToDXVertexFormat(mesh.GetVertexBuffer()->GetPositionFormat());
		g_InputLayout[g_InputLayoutCount].InputSlot            = g_VertexBufferCount;
		g_InputLayout[g_InputLayoutCount].AlignedByteOffset    = mesh.GetVertexBuffer()->GetPositionOffset();
		g_InputLayout[g_InputLayoutCount].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		g_InputLayout[g_InputLayoutCount].InstanceDataStepRate = 0;
		g_InputLayoutCount++;
	}

	if (mask & eVA_NORMAL)
	{
		g_InputLayout[g_InputLayoutCount].SemanticName         = "NORMAL";
		g_InputLayout[g_InputLayoutCount].SemanticIndex        = 0;
		g_InputLayout[g_InputLayoutCount].Format               = ConvertToDXVertexFormat(mesh.GetVertexBuffer()->GetNormalFormat());
		g_InputLayout[g_InputLayoutCount].InputSlot            = g_VertexBufferCount;	// specify which vertex buffer used
		g_InputLayout[g_InputLayoutCount].AlignedByteOffset    = mesh.GetVertexBuffer()->GetNormalOffset();
		g_InputLayout[g_InputLayoutCount].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		g_InputLayout[g_InputLayoutCount].InstanceDataStepRate = 0;
		g_InputLayoutCount++;
	}

	if (mask & eVA_VERTEXCOLOR)
	{
		g_InputLayout[g_InputLayoutCount].SemanticName         = "COLOR";
		g_InputLayout[g_InputLayoutCount].SemanticIndex        = 0;
		g_InputLayout[g_InputLayoutCount].Format               = ConvertToDXVertexFormat(mesh.GetVertexBuffer()->GetColorFormat());
		g_InputLayout[g_InputLayoutCount].InputSlot            = g_VertexBufferCount;	// specify which vertex buffer used
		g_InputLayout[g_InputLayoutCount].AlignedByteOffset    = mesh.GetVertexBuffer()->GetColorOffset();
		g_InputLayout[g_InputLayoutCount].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		g_InputLayout[g_InputLayoutCount].InstanceDataStepRate = 0;
		g_InputLayoutCount++;
	}

	for (int i = 0; i < 16; i++)
	{
		if (mask & (eVA_TEXCOORD0 << i))
		{
			g_InputLayout[g_InputLayoutCount].SemanticName         = "TEXCOORD";
			g_InputLayout[g_InputLayoutCount].SemanticIndex        = i;
			g_InputLayout[g_InputLayoutCount].Format               = ConvertToDXVertexFormat(mesh.GetVertexBuffer()->GetTexcoordFormat(i));
			g_InputLayout[g_InputLayoutCount].InputSlot            = g_VertexBufferCount;	// specify which vertex buffer used
			g_InputLayout[g_InputLayoutCount].AlignedByteOffset    = mesh.GetVertexBuffer()->GetTexcoordOffset(i);
			g_InputLayout[g_InputLayoutCount].InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
			g_InputLayout[g_InputLayoutCount].InstanceDataStepRate = 0;
			g_InputLayoutCount++;
		}
	}

	g_VertexBufferCount++;
}

bool SRenderHelper::CreateDeviceAndContext( HWND hwnd )
{
	IDXGIAdapter* pAdapter;
	SArray<IDXGIAdapter*> vAdapter;
	IDXGIFactory* pFactory = nullptr;

	if( FAILED( CreateDXGIFactory( __uuidof( IDXGIFactory ) , ( void** )&pFactory ) ) )
	{
		return false;
	}

	for( suInt32 i = 0; pFactory->EnumAdapters( i , &pAdapter ) != DXGI_ERROR_NOT_FOUND; ++i )
	{
		vAdapter.PushBack( pAdapter );
	}

	if( pFactory )	pFactory->Release();

#ifdef _DEBUG
	// get adapter display modes:https://msdn.microsoft.com/en-us/library/windows/desktop/ff476878(v=vs.85).aspx
	IDXGIOutput* pOutput = nullptr;

	for( suInt32 i = 0 , numAdapter = vAdapter.GetSize(); i < numAdapter; i++ )
	{
		pAdapter = vAdapter[i];

		SLog::Info( "Adapter:%d" , i );

		SArray<IDXGIOutput*> outputList;
		suInt32			outputIdx = 0;
		IDXGIOutput*	pOutput = nullptr;
		while( pAdapter->EnumOutputs( outputIdx , &pOutput ) != DXGI_ERROR_NOT_FOUND )
		{
			outputList.PushBack( pOutput );
			outputIdx++;
		}

		for( suInt32 j = 0 , outputNum = outputList.GetSize(); j < outputNum; j++ )
		{
			SLog::Info( "Adapter Output:%d" , j );

			pOutput = outputList[j];

			suInt32 numMode = 0;
			DXGI_MODE_DESC* displayModes = NULL;
			DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT;

			if( FAILED( pOutput->GetDisplayModeList( format , 0 , &numMode , NULL ) ) )	continue;

			displayModes = new DXGI_MODE_DESC[numMode];

			if( FAILED( pOutput->GetDisplayModeList( format , 0 , &numMode , displayModes ) ) )	continue;

			for( suInt32 k = 0; k < numMode; k++ )
			{
				const DXGI_MODE_DESC& desc = displayModes[k];

				SLog::Info( "Format:%s width:%d height:%d refreshRate:%d/%d" , Enum2Str( desc.Format ) , desc.Width , desc.Height , desc.RefreshRate.Numerator , desc.RefreshRate.Denominator );
			}
		}
	}
#endif

	// create a device and immediate context: https://msdn.microsoft.com/en-us/library/windows/desktop/ff476879(v=vs.85).aspx
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory( &scd , sizeof( scd ) );
	scd.BufferCount                        = 1;
	scd.BufferDesc.Width                   = 0;
	scd.BufferDesc.Height                  = 0;
	scd.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator   = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow                       = hwnd;
	scd.SampleDesc.Count                   = 1;
	scd.SampleDesc.Quality                 = 0;
	scd.Windowed                           = TRUE;
	scd.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags                              = 0;

	D3D_FEATURE_LEVEL FeatureLevelsRequested[] = {	D3D_FEATURE_LEVEL_11_1 ,
													D3D_FEATURE_LEVEL_11_0 ,
													D3D_FEATURE_LEVEL_10_1 ,
													D3D_FEATURE_LEVEL_10_0 ,
													D3D_FEATURE_LEVEL_9_3 ,
													D3D_FEATURE_LEVEL_9_2 ,
													D3D_FEATURE_LEVEL_9_1 };

	suInt32 createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL FeatureLevelsSupported;

	bool success = false;
	for( suInt32 i = 0 , numAdapter = vAdapter.GetSize(); i < numAdapter; i++ )
	{
		HRESULT hr = D3D11CreateDeviceAndSwapChain( vAdapter[i] ,
													D3D_DRIVER_TYPE_UNKNOWN ,
													NULL ,
													createDeviceFlags ,
													FeatureLevelsRequested ,
													_countof( FeatureLevelsRequested ) ,
													D3D11_SDK_VERSION ,
													&scd ,
													&g_SwapChain ,
													&g_Device ,
													&FeatureLevelsSupported ,
													&g_ImmediateContext );

		if( hr == E_INVALIDARG )
		{
			hr = D3D11CreateDeviceAndSwapChain( vAdapter[i] ,
												D3D_DRIVER_TYPE_UNKNOWN ,
												NULL ,
												createDeviceFlags ,
												&FeatureLevelsRequested[1] ,
												_countof( FeatureLevelsRequested ) - 1 ,
												D3D11_SDK_VERSION ,
												&scd ,
												&g_SwapChain ,
												&g_Device ,
												&FeatureLevelsSupported ,
												&g_ImmediateContext );
		}

		if( hr == S_OK )
		{
			success = true;
			break;
		}
	}

	return success;
}

void SRenderHelper::ReportDetailDebug()
{ 
#ifdef _DEBUG
	// ref: http://masterkenth.com/directx-leak-debugging/
	ID3D11Debug* pDebugDevice = nullptr;
	HRESULT hr = g_Device->QueryInterface( __uuidof( ID3D11Debug ) , reinterpret_cast< void** >( &pDebugDevice ) );
	if( hr == S_OK )
	{
		hr = pDebugDevice->ReportLiveDeviceObjects( D3D11_RLDO_DETAIL );
		SAFE_RELEASE( pDebugDevice );
	}
#endif
}

void SRenderHelper::BindTexture(eShaderStage stage, int slot, STexture2D* tex)
{
	if (!tex)	return;

	ID3D11ShaderResourceView* srv	  = tex->GetSRV();
	ID3D11SamplerState*       sampler = tex->GetSampler();
	switch (stage)
	{
	case eST_Vertex:
		g_ImmediateContext->VSSetShaderResources(slot, 1, &srv);
		g_ImmediateContext->VSSetSamplers(slot, 1, &sampler);
		break;
	case eST_Pixel:
		g_ImmediateContext->PSSetShaderResources(slot, 1, &srv);
		g_ImmediateContext->PSSetSamplers(slot, 1, &sampler);
		break;
	case eST_Geometry:
		g_ImmediateContext->GSSetShaderResources(slot, 1, &srv);
		g_ImmediateContext->GSSetSamplers(slot, 1, &sampler);
		break;
	case eST_Hull:
		g_ImmediateContext->HSSetShaderResources(slot, 1, &srv);
		g_ImmediateContext->HSSetSamplers(slot, 1, &sampler);
		break;
	case eST_Domain:
		g_ImmediateContext->DSSetShaderResources(slot, 1, &srv);
		g_ImmediateContext->DSSetSamplers(slot, 1, &sampler);
		break;
	case eST_Compute:
		g_ImmediateContext->CSSetShaderResources(slot, 1, &srv);
		g_ImmediateContext->CSSetSamplers(slot, 1, &sampler);
		break;
	}
}

void SRenderHelper::RenderStaticMesh(SStaticMesh& mesh, D3D11_PRIMITIVE_TOPOLOGY topology, const SShader& shader)
{
	ResetStream();
	AddMeshStream(mesh.GetMeshBuffer(), eVA_POSITION | eVA_NORMAL | eVA_TEXCOORD2 | eVA_TEXCOORD0 | eVA_IndexBuffer);

	g_ImmediateContext->IASetVertexBuffers(0, g_VertexBufferCount, g_VertexBuffer, g_VertexBufferStride, g_VertexBufferOffset);
	g_ImmediateContext->IASetIndexBuffer(g_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	if (ID3D11VertexShader* vs = shader.GetVertexShader())
	{
		g_ImmediateContext->VSSetShader(vs, nullptr, 0);
		g_ImmediateContext->IASetInputLayout(FindInputLayer(shader.GetVertexBlob()));
	}

	if (ID3D11PixelShader* ps = shader.GetPixelShader())
	{
		g_ImmediateContext->PSSetShader(ps, nullptr, 0);
	}

	if (ID3D11HullShader* hs = shader.GetHullShader())
	{
		g_ImmediateContext->HSSetShader(hs, nullptr, 0);
	}

	if (ID3D11DomainShader* ds = shader.GetDomainShader())
	{
		g_ImmediateContext->DSSetShader(ds, nullptr, 0);
	}

	if (ID3D11GeometryShader* gs = shader.GetGeometryShader())
	{
		g_ImmediateContext->GSSetShader(gs, nullptr, 0);
	}

	if (ID3D11ComputeShader* cs = shader.GetComputeShader())
	{
		g_ImmediateContext->CSSetShader(cs, nullptr, 0);
	}

	g_ImmediateContext->IASetPrimitiveTopology(topology);

	SRenderHelper::g_ImmediateContext->RSSetState(SRenderer::Get().GetRasterizerState());
	SRenderHelper::g_ImmediateContext->OMSetDepthStencilState(SRenderer::Get().GetDepthStencilState(), 0);

	for (int i = 0; i < mesh.GetSubmeshCount(); i++)
	{
		BindTexture(eST_Pixel, 0, mesh.GetDiffuseMap(i));
		BindTexture(eST_Pixel, 1, mesh.GetNormalMap(i));
		BindTexture(eST_Pixel, 2, mesh.GetMetallicMap(i));
		BindTexture(eST_Pixel, 3, mesh.GetRoughnessMap(i));
		BindTexture(eST_Pixel, 4, mesh.GetAOMap(i));

		g_ImmediateContext->DrawIndexed(mesh.GetSubmeshIndexCount(i), mesh.GetSubmeshStartIndex(i), mesh.GetSubmeshVertexOffset(i));
	}
}

void SRenderHelper::RenderFullScreen(const SShader& shader)
{
	g_ImmediateContext->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
	g_ImmediateContext->IASetIndexBuffer(nullptr, (DXGI_FORMAT)0, 0);
	g_ImmediateContext->IASetInputLayout(NULL);

	const SShader& fullscreenVS = SShader::FindShader("../data/shaders/fullscreenVS.hlsl");
	if (ID3D11VertexShader* vs = fullscreenVS.GetVertexShader())
	{
		g_ImmediateContext->VSSetShader(vs, nullptr, 0);
	}
	
	if (ID3D11PixelShader* ps = shader.GetPixelShader())
	{
		g_ImmediateContext->PSSetShader(ps, nullptr, 0);
	}

	g_ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_ImmediateContext->Draw(3, 0);
}

void SRenderHelper::SetRenderTarget(SRenderTexture* tex, bool useDepth)
{
	ID3D11RenderTargetView* rtv = tex->GetRenderTargetView();
	
	if (rtv)
	{
		ID3D11DepthStencilView* dsv = nullptr;
		if (useDepth)
		{
			dsv = tex->GetDepthStencilView();
		}
		else
		{
			g_ImmediateContext->OMGetRenderTargets(0, nullptr, &dsv);
		}
		g_ImmediateContext->OMSetRenderTargets(1, &rtv, dsv);
	}
}