cbuffer cbFrame : register(b0)
{
    float4x4 viewMatrix;
    float4x4 projMatrix;
    float4x4 viewProjMatrix;
    float    tanHalfFovX;
    float    tanHalfFovY;
    float    near;
    float    far;
    float    aspectRatio;
    float3   cameraWorldPos;
};

Texture2D albedoMap    : register(t0);
Texture2D normalMap    : register(t1);
Texture2D metallicMap  : register(t2);
Texture2D roughnessMap : register(t3);
Texture2D aoMap        : register(t4);

sampler   albedoSampler   : register(s0);
sampler   normalSampler    : register(s1);
sampler   metallicSampler  : register(s2);
sampler   roughnessSampler : register(s3);
sampler   aoSampler        : register(s4);

#define PI 3.141592653589793238f
#define TWO_PI 6.283185307179586477f