#include "common.hlsli"

float4 main(PS_INPUT input) : SV_Target
{
    return input.Color;
}