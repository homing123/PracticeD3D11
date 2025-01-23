#include "Common.hlsli"

cbuffer IBLCbuffer : register(b0)
{
	float3 diffuse;
	float dummy;
	float3 specular;
	float dummy2;
}

float4 main(PSInput i) : SV_TARGET
{
	float3 view = normalize(eyeWorld - i.posWorld);

	float3 color = float3(0, 0, 0);
	float3 diffuseIBL = irradianceIBLTex.Sample(LinearClamp, i.normalWorld).rgb * diffuse;
	float3 specularIBL = envIBLTex.Sample(LinearClamp, reflect(-view, i.normalWorld)).rgb * specular;
	color += (diffuseIBL + specularIBL) * iblStrength;

	return float4(color, 1);
}