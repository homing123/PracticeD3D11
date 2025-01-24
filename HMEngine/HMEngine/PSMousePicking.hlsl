#include "Common.hlsli"

cbuffer MousePickingCBuffer : register(b0)
{
	uint4 color;
}

uint4 main(PSInput i) : SV_TARGET
{
	return uint4(color.rgb, 1);
}