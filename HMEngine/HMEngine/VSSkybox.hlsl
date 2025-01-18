#include "Common.hlsli"
struct VSSkyboxInput
{
	float3 posModel : POSITION;
	float2 uv : TEXCOORD;
};
struct PSSkyboxInput
{
	float4 posProj : SV_POSITION;
	float3 posModel : POSITION;
};

PSSkyboxInput main(VSSkyboxInput i)
{
	PSSkyboxInput o;
	float4 posView = mul(float4(i.posModel, 0), view); //È¸Àü¸¸
	o.posProj = mul(float4(posView.xyz,1), proj);
	o.posModel = i.posModel;
	return o;
}