#include "Util.h"

const void Util::NDCToScreen(const Vector2 normalizedPos, const int width, const int height, int& out_ScreenX, int& out_ScreenY)
{
	out_ScreenX = normalizedPos.x == 1 ? int(width * normalizedPos.x) - 1 : int(width * normalizedPos.x);
	out_ScreenY = normalizedPos.y == 1 ? int(height * normalizedPos.y) - 1 : int(height * normalizedPos.y);
}
const void Util::IdxToUINT3Color(const int idx, UINT& out_r, UINT& out_g, UINT& out_b)
{
	out_r = idx % 256;
	out_g = (idx / 256) % 256;
	out_b = idx / 65536; // idx / (256 * 256)
}
const int Util::UINT3ColorToIdx(const UINT r, const UINT g, const UINT b)
{
	return r + g * 256 + b * 65536;
}