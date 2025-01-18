#ifndef __COMMON_HLSL__
#define __COMMON_HLSL__

#define MAX_LIGHTS 3
#define NUM_DIR_LIGHTS 1
#define NUM_POINT_LIGHTS 1
#define NUM_SPOT_LIGHTS 1


//모든 셰이더에서 공통으로 사용하는것들
struct Light
{
	float3 strength ;       // 12
	float fallOffStart;                 // 4
	float3 direction; // 12
	float fallOffEnd;                      // 4
	float3 position; // 12
	float spotPower;                     // 4
};
cbuffer GlobalCBuffer : register(b10)
{
	float4x4 view;
	float4x4 proj;
	float4x4 invProj;
	float4x4 viewProj;
	float4x4 invViewProj;

	float3 eyeWorld;
	float iblStrength;

	Light lights[MAX_LIGHTS];
};
cbuffer TransformCBuffer : register(b11)
{
	float4x4 world;
	float4x4 worldIT;
};

SamplerState LinearWrap : register(s0);
SamplerState LinearClamp : register(s1);

TextureCube envIBLTex : register(t10);
TextureCube irradianceIBLTex : register(t11);
TextureCube specularIBLTex : register(t12);
Texture2D brdfIBLTex : register(t13);



struct VSInput
{
	float3 posModel : POSITION;
	float3 normalModel : NORMAL;
	float2 uv : TEXCOORD;
	float3 tangentModel : TANGENT;
};
struct PSInput
{
	float4 posProj : SV_POSITION;
	float3 posWorld : POSITION;
	float3 normalWorld : NORMAL;
	float2 uv : TEXCOORD;
	float3 tangentWorld : TANGENT;
};
#endif