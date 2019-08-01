#ifndef _COMMON_CONST_
#define _COMMON_CONST_

float4x4 viewMatrix;
float4x4 projMatrix;
float4x4 viewProjMatrix;
float    tanHalfFovX;
float    tanHalfFovY;
float    near;
float    far;
float    aspectRatio;
float3   cameraWorldPos;

Texture2D albedoMap;
Texture2D normalMap;
Texture2D metallicMap;
Texture2D roughnessMap;
Texture2D aoMap;

sampler   albedoSampler;
sampler   normalSampler;
sampler   metallicSampler;
sampler   roughnessSampler;
sampler   aoSampler;

#ifndef PI
#define PI 3.141592653589793238f
#endif

#ifndef TWO_PI
#define TWO_PI 6.283185307179586477f
#endif

#endif