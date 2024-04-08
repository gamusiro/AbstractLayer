#include "common.hlsli"

Texture2D g_texture		: register(t0);
SamplerState g_sampler	: register(s0);

float4 main(PS_INPUT input) : SV_Target
{
    float4 texColor = g_texture.Sample(g_sampler, input.TexCoord);
    
    float4 outColor = texColor * input.Color;
    
    if (outColor.a < 0.1f)
        discard;
	
    return outColor;

}