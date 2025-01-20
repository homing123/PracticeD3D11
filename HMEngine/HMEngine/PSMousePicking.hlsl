#include "Common.hlsli"

cbuffer MousePickingCBuffer : register(b0)
{
	uint3 color;
	int idx;
}

uint4 main(PSInput i) : SV_TARGET
{
	return uint4(color, 1);
}