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
	const static void DrawMaterial(ComPtr<ID3D11DeviceContext>& context, MaterialCBuffer* pMat, ComPtr<ID3D11Buffer>& cbuffer, const E_MatKind kind);
};