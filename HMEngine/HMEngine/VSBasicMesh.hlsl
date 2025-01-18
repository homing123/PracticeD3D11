#include "Common.hlsli"

PSInput main(VSInput i)
{
	PSInput o;
	float4 posWorld = mul(float4(i.posModel, 1), world);
	o.posWorld = posWorld.xyz;
	o.posProj = mul(posWorld, viewProj);

	o.uv = i.uv;

	o.normalWorld = normalize(mul(float4(i.normalModel, 0), worldIT).xyz);

	o.tangentWorld = normalize(mul(float4(i.tangentModel, 0), world).xyz);

	return o;
}