#include "Common.hlsli"
#include "BillboardPointImage.hlsli"

BillboardPI_GSInput main(BillboardPI_VSInput i)
{
	BillboardPI_GSInput o;
	o.posWorld = mul(float4(i.posModel, 1), world);
	return o;
}