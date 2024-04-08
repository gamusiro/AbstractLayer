struct VS_INPUT
{
    float4 Position : POSITION;
    float4 Normal   : NORMAL;
    float2 TexCoord : TEXCOORD;
};

struct PS_INPUT
{
    float4 Position : SV_Position;
    float4 WorldPos : POSITION;
    float4 Normal   : NORMAL;
    float4 Color    : COLOR;
    float2 TexCoord : TEXCOORD;
};

cbuffer SHADER_PARAM : register(b0)
{
    matrix world;
    matrix view;
    matrix proj;
};

cbuffer SHADER_MATERIAL : register(b1)
{
    float4  albedo;
    float   metallic;
    float   roughness;
};
