#include "Common.hlsli"

struct PSSkyboxInput
{
	float4 posProj : SV_POSITION;
	float3 posModel : POSITION;
};

float4 main(PSSkyboxInput i) : SV_TARGET
{
	float3 color = envIBLTex.Sample(LinearWrap, i.posModel) * iblStrength;
	return float4(color, 1.0f);
}