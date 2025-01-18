#pragma once

#include <vector>
#include "D3DUtil.h"

using namespace std;
using Microsoft::WRL::ComPtr;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Vector3;

enum E_MatKind
{
	PBR,
	BlinnPhong,
	Skybox,
};
struct MaterialCBuffer
{

};
__declspec(align(256)) struct PBRCBuffer : MaterialCBuffer
{
	int useAlbedo = 0;
	int useNormal = 0;
	int useAO = 0;
	int useMetalic = 0;
	int useRoughness = 0;
	int useEmissive = 0;
	Vector2 dummy;
};

__declspec(align(256)) struct BlinnPhongCBuffer : MaterialCBuffer
{
	Vector3 ambient;
	float shininess;
	Vector3 diffuse;
	float dummy;
	Vector3 specular;
	float dummy2;
};

__declspec(align(256)) struct SkyboxCBuffer : MaterialCBuffer
{

};