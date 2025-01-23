#include "BlinnPhong.hlsli"

#define MAX_LIGHTS 3
#define NUM_DIR_LIGHTS 1
#define NUM_POINT_LIGHTS 1
#define NUM_SPOT_LIGHTS 1

float4 main(PSInput i) : SV_TARGET
{
	return float4(1,1,1,1);
	float3 view = normalize(eyeWorld - i.posWorld);

	float3 color = float3(0, 0, 0);

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