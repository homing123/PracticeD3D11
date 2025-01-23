#include "BlinnPhong.hlsli"

float4 main(PSInput i) : SV_TARGET
{
	float3 view = normalize(eyeWorld - i.posWorld);

	float3 color = float3(0, 0, 0);

	int idx = 0;

	[unroll]
	for (int idx = 0; idx < MAX_LIGHTS; idx++)
	{
		if (lights[idx].LightKind == LIGHT_DIRECTIONAL)
		{
			color += ComputeDirectionalLight(lights[idx], material, i.normalWorld, view);
		}
		else if (lights[idx].LightKind == LIGHT_POINT)
		{
			color += ComputePointLight(lights[idx], material, i.posWorld, i.normalWorld, view);
		}
		else if (lights[idx].LightKind == LIGHT_SPOT)
		{
			color += ComputeSpotLight(lights[idx], material, i.posWorld, i.normalWorld, view);
		}
	}
	

	return float4(color, 1);
}