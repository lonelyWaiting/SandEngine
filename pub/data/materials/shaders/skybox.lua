#include "commonConst.hlsl"
#include "fullscreenVS.hlsl"

cbuffer skyCB
{
    float4x4 cameraToWorld;
}

Texture2D envMap<default="";addressU="";addressV="";addressW="";filter="";>

float4 ps_main(float2 uv : TEXCOORD0) : SV_Target
{
    float3 camera_world_pos = mul(float4(0.0f,0.0f,0.0f,1.0f), cameraToWorld).xyz;
    float2 ndc = float2(2 * uv.x - 1.0f , 1.0f - 2 * uv.y);
    float3 pixel_world_pos = mul(float4(ndc.x * tanHalfFovX, ndc.y * tanHalfFovY, 1.0f, 1.0f), cameraToWorld).xyz;

    float3 ray_world = normalize(pixel_world_pos - camera_world_pos);

    float theta = acos(clamp(ray_world.y,-1.0f, 1.0f));
    float phi = atan2(ray_world.z, ray_world.x);
    phi = phi < 0.0f ? phi + TWO_PI : phi;

    float4 c = envMap.Sample(envMapSampler, float2(phi / TWO_PI, theta / PI));

    // tone mapping
    c = c / (c + 1.0f);
    // gamma correct
    c = pow(c, 1.0f/2.2f);

    return c;
}

Pass
{
    #pragma vert vs_main
    #pragma fragment ps_main

    local blend_state = 
    {
        -- blend state
        blendEnable           = true,
        srcBlend              = MATBlendFactor.SrcAlpha,
        destBlend             = MATBlendFactor.InvSrcAlpha,
        blendOp               = MATBlendOperator.Add,
        srcBlendAlpha         = MATBlendFactor.One,
        destBlendAlpha        = MATBlendFactor.Zero,
        blendOpAlpha          = MATBlendOperator.Add,
        renderTargetWriteMask = 0xff,
    }

    local rasterizer_state =
    {
        -- rasterzier state
        fullMode = MATFullMode.Solid,
        cullMode = MATCullMode.Back,
    }

    local depth_stencil_state = 
    {
        cullMode = "point",
        depthTest = true,
    }
}