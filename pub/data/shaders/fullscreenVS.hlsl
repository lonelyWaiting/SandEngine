struct vs_output
{
    float2 uv       : TEXCOORD0;
    float4 vertex   : SV_POSITION;
};

vs_output vs_main(uint id : SV_VERTEXID)
{
    vs_output output;

    // generate clip space position
    output.vertex.x = (float)(id / 2) * 4.0f - 1.0f;
    output.vertex.y = (float)(id % 2) * 4.0f - 1.0f;
    output.vertex.z = 0.0f;
    output.vertex.w = 1.0f;

    // texture coordinate
    output.uv.x = (float)(id / 2) * 2.0f;
    output.uv.y = 1.0f - (float)(id % 2) * 2.0f;
    
    return output;
}