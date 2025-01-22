#pragma once

#include "directxtk/SimpleMath.h"

using DirectX::SimpleMath::Vector2;

class Util
{
public:
	const static void NDCToScreen(const Vector2 normalizedPos, const int width, const int height, int& out_ScreenX, int& out_ScreenY);
	const static void IdxToUINT3Color(const int idx, UINT& out_r, UINT& out_g, UINT& out_b);
	const static int UINT3ColorToIdx(const UINT r, const UINT g, const UINT b);
};