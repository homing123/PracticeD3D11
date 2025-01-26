#include "Common.hlsli"
#include "BillboardPointImage.hlsli"

cbuffer MousePickingCBuffer : register(b0)
{
	uint4 color;
}

BillboardPI_PSOutput main(BillboardPI_PSInput i) : SV_TARGET
{
	float4 texColor = Tex.Sample(LinearClamp, i.uv);
	BillboardPI_PSOutput o;
	o.MainColor = float4(texColor.rgb, 1.0f);
	o.MousePickingColor = uint4(color.rgb, 255);
	//clip(texColor.r > 0.5f ? -1 : 1); //검정배경화면때문에 안보인다.
	return o;
}