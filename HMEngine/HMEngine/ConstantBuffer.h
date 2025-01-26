#pragma once

#include <directxtk/SimpleMath.h>

using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;

#define MAX_LIGHTS 8
#define LIGHT_CBUFFER_SLOT 12
#define TRANSFORM_CBUFFER_SLOT 11
#define GLOBAL_CBUFFER_SLOT 10
#define IBL_TEX_SLOT 10
#define IBL_TEX_COUNT 4

enum E_LightType
{
	None = 0,
	Directional = 1,
	Point = 2,
	Spot = 3,
};
struct LightInfo
{
	E_LightType LightType;
	Vector3 dummy;
	Vector3 strength = Vector3(0.0f);
	float fallOffStart = 0;
	Vector3 direction = Vector3(0, -1, 0);
	float fallOffEnd = 10;
	Vector3 position = Vector3(0, 0, 0);
	float spotPower = 100;
};

__declspec(align(256)) struct LightCBuffer // b12
{
	LightInfo lights[MAX_LIGHTS];
	void Init() {
		for (int i = 0; i < MAX_LIGHTS; i++)
		{
			lights[i].LightType = None;
		}
	}
};
__declspec(align(256)) struct GlobalCBuffer //b10
{
	Matrix view;
	Matrix proj;
	Matrix invProj;
	Matrix viewProj;
	Matrix invViewProj;

	Vector3 eyeWorld;
	float iblStrength = 1;
};

__declspec(align(256)) struct TransformCBuffer //b11
{
	Matrix world;
	Matrix worldIT;
};
