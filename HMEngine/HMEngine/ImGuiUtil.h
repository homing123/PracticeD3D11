#pragma once
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include "Transform.h"
#include "Material.h"

class ImGuiUtil
{
public:
	const static void DrawTransform(Transform* pTF);
	const static void DrawMaterial(MaterialCBuffer* pMat, const E_MatKind kind);
};