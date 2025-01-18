#include "Common.hlsli"

cbuffer MousePickingCBuffer : register(b0)
{
	float3 color;
	int idx;
}

float4 main(PSInput i) : SV_TARGET
{
	return float4(color, 1);
}