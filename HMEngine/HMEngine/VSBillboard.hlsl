#include "Common.hlsli"
#include "Billboard.hlsli"

Billboard_GSInput main(Billboard_VSInput i)
{
	Billboard_GSInput o;
	o.posWorld = mul(float4(i.posModel, 1), world);
	return o;
}