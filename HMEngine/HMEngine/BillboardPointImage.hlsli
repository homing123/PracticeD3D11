
struct BillboardPI_VSInput
{
	float3 posModel : POSITION;
};
struct BillboardPI_GSInput
{
	float4 posWorld : SV_POSITION;
};
struct BillboardPI_PSInput
{
	float4 posProj : SV_POSITION;
	float2 uv : TEXCOORD;
};
struct BillboardPI_PSOutput
{
	float4 MainColor : SV_Target0;
	uint4 MousePickingColor : SV_Target1;
};

Texture2D Tex : register(t0);