#pragma once
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "Transform.h"

class ImGuiUtil
{
public:
	const static void DrawTransform(Transform* pTF);
};