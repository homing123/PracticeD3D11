#include "Common.hlsli"
#include "BillboardPointImage.hlsli"

cbuffer MousePickingCBuffer : register(b0)
{
	uint4 color;
}

BillboardPI_PSOutput main(BillboardPI_PSInput i) : SV_TARGET
{
	float3 texColor = Tex.Sample(LinearClamp, i.uv).rgb;
	BillboardPI_PSOutput o;
	o.MainColor = float4(texColor, 1.0f);
	o.MousePickingColor = uint4(color.rgb, 255);
	return o;
}