#include "BlinnPhong.hlsli"

#define MAX_LIGHTS 3
#define NUM_DIR_LIGHTS 1
#define NUM_POINT_LIGHTS 1
#define NUM_SPOT_LIGHTS 1

float4 main(PSInput i) : SV_TARGET
{
	float3 view = normalize(eyeWorld - i.posWorld);

	float3 color = float3(0, 0, 0);

	float3 diffuseIBL = irradianceIBLTex.Sample(LinearClamp, i.normalWorld).rgb * material.diffuse;
	float3 specularIBL = specularIBLTex.Sample(LinearClamp, reflect(-view, i.normalWorld)).rgb * material.specular;
	color += (diffuseIBL + specularIBL) * iblStrength;

	return float4(color, 1);
	int idx = 0;

	[unroll]
	for (idx = 0; idx < NUM_DIR_LIGHTS; idx++)
	{
		color += ComputeDirectionalLight(lights[idx], material, i.normalWorld, view);
	}

	[unroll]
	for (idx = NUM_DIR_LIGHTS; idx < NUM_DIR_LIGHTS + NUM_POINT_LIGHTS; idx++)
	{
		color += ComputePointLight(lights[idx], material, i.posWorld, i.normalWorld, view);
	}

	[unroll]
	for (idx = NUM_DIR_LIGHTS + NUM_POINT_LIGHTS; idx < NUM_DIR_LIGHTS + NUM_POINT_LIGHTS + NUM_SPOT_LIGHTS; idx++)
	{
		color += ComputeSpotLight(lights[idx], material, i.posWorld, i.normalWorld, view);
	}

	return float4(color, 1);
}