
struct Billboard_VSInput
{
	float3 posModel : POSITION;
};
struct Billboard_GSInput
{
	float4 posWorld : SV_POSITION;
};
struct Billboard_PSInput
{
	float4 posProj : SV_POSITION;
	float2 uv : TEXCOORD;
};

Texture2D Tex : register(t0);