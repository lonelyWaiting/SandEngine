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

#define PI 3.141592653589793238f
#define TWO_PI 6.283185307179586477f

Texture2D BRDFLutMap : register(t5);
Texture2D IrradianceMap : register(t6);
Texture2D prefilterMap : register(t7);

sampler samplerBRDFLut;
sampler samplerIrradiace;
sampler samplerPrefilter;

cbuffer cbIBL : register(b1)
{
    float4x4 worldMatrix;
};

struct PointLight
{
    float3 lightPos;
    float  padding;
    float3 lightColor;
    float  padding2;
};

cbuffer cbLight : register(b2)
{
    PointLight light[4];
    float customRoughness;
    float customMetallic;
    float2 padding;
};

struct vs_input
{
    float3 position     : POSITION;
    float3 normal       : NORMAL;
    float3 tangent      : TEXCOORD2; 
    float2 uv           : TEXCOORD0;
};

struct vs_output
{
    float3 world_normal     : NORMAL;
    float3 world_tangent    : TEXCOORD2;
    float2 uv               : TEXCOORD;
    float3 world_pos        : POSITION;
    float4 pos              : SV_POSITION;
};

vs_output vs_main(vs_input input)
{
    vs_output output;
    output.world_pos        = mul(float4(input.position,1.0f), worldMatrix).xyz;
    output.world_normal     = normalize(mul(float4(input.normal,0.0f), worldMatrix).xyz);
    output.world_tangent    = normalize(mul(float4(input.tangent, 0.0f), worldMatrix).xyz);
    output.pos              = mul(float4(output.world_pos, 1.0f), viewProjMatrix);
    output.uv               = input.uv;

    return output;
}

float3 normalMapping(float3 sampleNormal , float3 world_normal, float3 world_tangent)
{
    sampleNormal = 2 * sampleNormal - 1;

	float3 N = world_normal;
	float3 T = normalize(world_tangent - dot(world_tangent, N) * N);
	float3 B = normalize(cross(N , T));
    float3x3 TBN = float3x3( T , B , N );

    return normalize(mul(sampleNormal, TBN));
}

float GGX(float NdotH, float roughness)
{
    float alpha = roughness * roughness;
    float s = NdotH * NdotH * (alpha * alpha - 1) + 1;
    return alpha * alpha / max(PI * s * s, 0.0001f);
}

float GeometrySmith(float NdotL, float NdotV, float roughness)
{
    float k = (roughness + 1) * (roughness + 1) / 8;
    float ggx1 = NdotL / (NdotL * (1 - k) + k);
    float ggx2 = NdotV / (NdotV * (1 - k) + k);

    return ggx1 * ggx2;
}

float3 FresnelShclick(float3 F0, float LdotH)
{
    return F0 + (1 - F0) * pow((1 - LdotH), 5);
}

float3 FresnelSchlickRoughness(float cosTheta, float3 F0, float roughness)
{
    return F0 + (max(float3(1.0f - roughness, 1.0f - roughness, 1.0f - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

float2 CartesianToSpherical(float3 dir)
{
    float theta = acos(clamp(dir.y, -1.0f, 1.0f));
    float phi   = atan2(dir.z, dir.x);
    phi = phi < 0.0f ? phi + TWO_PI : phi;
    return float2(phi / TWO_PI, theta / PI);
}

float4 ps_main(vs_output input) : SV_Target
{
    float3 N = input.world_normal;
    float3 V = normalize(cameraWorldPos - input.world_pos);

    // float3 albedo = float3(1.0f,1.0f,1.0f);
    // float metallic = customMetallic;
    // float roughness = customRoughness;
    // float AO = 1.0f;

    // srgb => linear
    float3 albedo  = albedoMap.Sample(albedoSampler, input.uv).xyz;    
    albedo         = pow(albedo, 2.2f);
    float metallic  = metallicMap.Sample(metallicSampler, input.uv).r;
    float roughness = roughnessMap.Sample(roughnessSampler, input.uv).r;
    float AO        = aoMap.Sample(aoSampler, input.uv).r;
    float3 sampleNormal = normalMap.Sample(normalSampler, input.uv).xyz;
    N               = normalMapping(sampleNormal, N, input.world_tangent);

    float3 F0 = float3(0.04f, 0.04f, 0.04f);
    F0 = lerp(F0, albedo, metallic);

    float NdotV = saturate(dot(N,V));

    float3 Lo = float3(0.0f,0.0f,0.0f);
    for(int i = 0; i < 4; i++)
    {
        float3 L = normalize(light[i].lightPos - input.world_pos);
        float3 H = normalize(L + V);

        float d = length(light[i].lightPos - input.world_pos);
        float3 radiance = light[i].lightColor * 1.0f / (d * d);

        float NdotH = saturate(dot(N,H));
        float NdotL = saturate(dot(N,L));
        float LdotH = saturate(dot(L,H));

        float D  = GGX(NdotH, roughness);
        float G  = GeometrySmith(NdotL, NdotV, roughness);
        float3 F = FresnelShclick(F0, LdotH);
        
        // energy conversation
        float3 ks = F;
        float3 kd = 1.0f - ks;
        kd *= 1.0f - metallic;
        
        Lo += radiance * (kd * albedo / PI + D * G * F / max(4.0f * NdotL * NdotV, 0.001f)) * NdotL; 
    }

    float3 F = FresnelSchlickRoughness(NdotV, F0, roughness);

    float3 ks = F;
    float3 kd = 1.0 - ks;
    kd *= 1.0 - metallic;
    
    float3 irradiance = IrradianceMap.Sample(samplerIrradiace, CartesianToSpherical(N)).rgb;
    float3 diffuse    = irradiance * albedo;
    
    float3 R        = reflect(-V, N);
    const float MAX_REFLECTION_LOD = 10.0;
    float3 prefilteredColor = prefilterMap.SampleLevel(samplerPrefilter, CartesianToSpherical(R), roughness * MAX_REFLECTION_LOD).rgb;
    float2 envBRDF  = BRDFLutMap.Sample(samplerBRDFLut, float2(NdotV, roughness)).rg;
    float3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);
    float3 ambient = (kd * diffuse + specular) * AO; 

    Lo = Lo + ambient;
    // tonemapping
    Lo = Lo / (Lo + 1.0f);
    // gamma correct
    Lo = pow(Lo, 1.0f / 2.2f);

    return float4(Lo, 1.0f);
}