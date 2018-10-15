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

Texture2D albedoMap   : register(t0);
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

Texture2D BRDFLutMap    : register(t5);
Texture2D IrradianceMap : register(t6);
Texture2D prefilterMap  : register(t7);

sampler samplerBRDFLut   : register(s5);
sampler samplerIrradiace : register(s6);
sampler samplerPrefilter : register(s7);

cbuffer cbIBL : register(b1)
{
    float4x4 worldMatrix;
    float customRoughness;
    float customMetallic;
    float2 padding;
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
    PointLight light[3];
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
    output.world_normal     = mul(float4(input.normal,1.0f), worldMatrix).xyz;
    output.world_tangent    = mul(float4(input.tangent, 1.0f), worldMatrix).xyz;
    output.pos              = mul(float4(output.world_pos, 1.0f), viewProjMatrix);
    output.uv               = input.uv;

    return output;
}

float3 normalMapping(float3 tangent_space_normal , float3 world_normal, float3 world_tangent)
{
    tangent_space_normal = 2 * tangent_space_normal - 1;
	float3 N = world_normal;
	float3 T = normalize( world_tangent - dot( world_tangent , N ) * N );
	float3 B = cross( N , T );

    /*
        Tx Ty Tz
        Bx By Bz
        Nx Ny Nz
    */
    float3x3 TBN = float3x3( T , B , N );

    return normalize(mul( tangent_space_normal , TBN ));
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

    // srgb => linear
    // float3 albedo  = albedoMap.Sample(albedoSampler, input.uv).xyz;
    float3 albedo = float3(1.0f,1.0f,1.0f);
    albedo         = pow(albedo, 2.2f);
    
    // float metallic  = metallicMap.Sample(metallicSampler, input.uv).r;
    float metallic = customMetallic;
    // float roughness = roughnessMap.Sample(roughnessSampler, input.uv).r;
    float roughness = customRoughness;
    // float AO        = aoMap.Sample(aoSampler, input.uv).r;
    float AO = 1.0f;
    // float3 sampleNormal = normalMap.Sample(normalSampler, input.uv).xyz;
    float3 sampleNormal = float3(0.0f,0.0f,0.0f);
    N               = normalMapping(sampleNormal, N, input.world_tangent);
    float3 R        = reflect(-V, N);

    float3 F0 = float3(0.04f, 0.04,0.04f);
    F0 = lerp(F0, albedo, metallic);

    float NdotV = saturate(dot(N,V));

    float3 Lo = float3(0.0f,0.0f,0.0f);
    for(int i = 0; i < 3; i++)
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
        float3 kd = 1.0f - F;
        kd *= 1.0f - metallic;
        
        Lo += light[i].lightColor * (kd * albedo / PI + D * G * F / max(4.0f * NdotL * NdotV, 0.001f)) * NdotL; 
    }

    float3 F = FresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    float3 ks = F;
    float3 kd = 1.0 - ks;
    kd *= 1.0 - metallic;	  
    
    float3 irradiance = IrradianceMap.Sample(samplerIrradiace, CartesianToSpherical(N)).rgb;
    float3 diffuse    = irradiance * albedo;
    
    const float MAX_REFLECTION_LOD = 10.0;
    float3 prefilteredColor = prefilterMap.SampleLevel(samplerPrefilter, CartesianToSpherical(R), roughness * MAX_REFLECTION_LOD).rgb;
    float2 envBRDF  = BRDFLutMap.Sample(samplerBRDFLut, float2(max(NdotV, 0.0), roughness)).rg;
    float3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);
    
    float3 ambient = (kd * diffuse + specular) * AO; 

    Lo = Lo + ambient;

    // tonemapping
    Lo = Lo / (Lo + 1.0f);
    // gamma correct
    Lo = pow(Lo, 1.0f / 2.2f);

    return float4(Lo, 1.0f);
}