#include "common.hlsli"


const static float4 lightDirection = float4(0, -1, 1, 0);

PS_INPUT main( VS_INPUT input )
{
    PS_INPUT output;
    float4 worldPosition = mul(input.Position, world);
    matrix viewProjection = mul(view, proj);
    
    // 頂点の向きを計算する
    float4 worldNormal = mul(float4(input.Normal.xyz, 0.0f), world);
    worldNormal = normalize(worldNormal);
    
    // ライトの向きを計算
    float light = -dot(worldNormal.xyz, lightDirection.xyz);
    
    output.Position = mul(worldPosition, viewProjection);
    output.Color = float4(albedo.xyz * light, 1.0);
    
	return output;
}