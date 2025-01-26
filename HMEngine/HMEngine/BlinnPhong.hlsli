#include "Common.hlsli"

struct BlinnPhongMaterial
{
	float3 ambient;
	float shininess;
	float3 diffuse;
	float dummy;
	float3 specular;
	float dummy2;
};

Texture2D albedoTex : register(t0);
cbuffer BlinnPhongPSCBuffer : register(b0)
{
	BlinnPhongMaterial material;
};

float3 BlinnPhong(float3 lightStrength, float3 lightDir, float3 normal, float3 viewDir, BlinnPhongMaterial mat)
{
	float3 halfDir = normalize(lightDir + viewDir);
	float ndoth = max(0, dot(halfDir, normal));
	float3 specular = pow(ndoth, mat.shininess) * mat.specular;
	return (mat.diffuse + specular) * lightStrength;
}
float3 ComputeDirectionalLight(Light light, BlinnPhongMaterial mat, float3 normal, float3 viewDir)
{
	float3 lightDir = -light.direction;
	float3 lightStrength = max(0, dot(normal, lightDir)) * light.strength;

	return mat.ambient + BlinnPhong(lightStrength, lightDir, normal, viewDir, mat);
}

float CalcAttenuation(float d, float fallOffStart, float fallOffEnd)
{
	return saturate((fallOffEnd - d) / (fallOffEnd - fallOffStart));
}
float3 ComputePointLight(Light light, BlinnPhongMaterial mat, float3 pos, float3 normal, float3 viewDir)
{
	float3 lightVec = light.position - pos;
	float distance = length(lightVec);
	if (distance > light.fallOffEnd)
	{
		return float3(0, 0, 0);
	}
	else
	{
		float3 lightDir = lightVec / distance;
		float att = CalcAttenuation(distance, light.fallOffStart, light.fallOffEnd);
		float ndotl = max(0, dot(normal, lightDir));
		float3 lightStrength = light.strength * att * ndotl;

		return BlinnPhong(lightStrength, lightDir, normal, viewDir, mat);
	}
}
float3 ComputeSpotLight(Light light, BlinnPhongMaterial mat, float3 pos, float3 normal, float3 viewDir)
{
	float3 lightVec = light.position - pos;
	float distance = length(lightVec);
	if (distance > light.fallOffEnd)
	{
		return float3(0, 0, 0);
	}
	else
	{
		float3 lightDir = lightVec / distance;
		float att = CalcAttenuation(distance, light.fallOffStart, light.fallOffEnd);
		float ndotl = max(0, dot(normal, lightDir));
		float spotFactor = pow(max(0, dot(lightDir, -light.direction)), light.spotPower);
		float3 lightStrength = light.strength * att * ndotl * spotFactor;

		return BlinnPhong(lightStrength, lightDir, normal, viewDir, mat);
	}
}