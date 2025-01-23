#pragma once

#include <directxtk/SimpleMath.h>

using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;

#define MAX_LIGHTS 3
#define TRANSFORM_CBUFFER_SLOT 11
#define GLOBAL_CBUFFER_SLOT 10
#define IBL_TEX_SLOT 10
#define IBL_TEX_COUNT 4
struct Light
{
	Vector3 strength = Vector3(0.0f);
	float fallOffStart = 0;
	Vector3 direction = Vector3(0, -1, 0);
	float fallOffEnd = 10;
	Vector3 position = Vector3(0, 0, 0);
	float spotPower = 100;
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

	Light lights[MAX_LIGHTS];
};

__declspec(align(256)) struct TransformCBuffer //b11
{
	Matrix world;
	Matrix worldIT;
};
