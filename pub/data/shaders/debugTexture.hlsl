struct vs_output
{
    float2 uv       : TEXCOORD0;
    float4 vertex   : SV_POSITION;
};

Texture2D tex;
sampler texSampler;

float4 ps_main(vs_output input) : SV_Target
{
    return tex.Sample(texSampler,input.uv);
}