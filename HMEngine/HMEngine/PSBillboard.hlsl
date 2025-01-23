#include "Common.hlsli"
#include "Billboard.hlsli"

float4 main(Billboard_PSInput i) : SV_TARGET
{
	float3 color = Tex.Sample(LinearClamp, i.uv).rgb;
	return float4(color, 1.0f);
}