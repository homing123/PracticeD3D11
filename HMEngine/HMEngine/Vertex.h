#pragma once

#include <directxtk/SimpleMath.h>

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;

struct Vertex
{
	Vector3 pos;
	Vector3 normal;
	Vector2 uv;
	Vector3 tangent;
};