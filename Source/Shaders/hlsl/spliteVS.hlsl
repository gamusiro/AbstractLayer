#include "common.hlsli"


PS_INPUT main( VS_INPUT input )
{
    PS_INPUT output;
    float4 worldPosition  = mul(input.Position, world);
    matrix viewProjection = mul(view, proj);
    
    
    output.Position = mul(worldPosition, proj);
    output.Normal   = float4(input.Normal.xyz, 1.0);
    output.Color    = albedo;
    output.TexCoord = input.TexCoord;
    
	return output;
}