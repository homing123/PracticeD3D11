#pragma once
#include "Material.h"
#include "D3DUtil.h"

__declspec(align(256)) struct SkyboxCBuffer
{

};


class SkyboxMaterial : public Material
{
public:

	void CreateCBuffer(ComPtr<ID3D11Device>& device)
	{
	}
	void UpdateCBuffer(ComPtr<ID3D11DeviceContext>& context)
	{
	}
	void SetVector4(const string& name, const Vector4& vt4) {}
	void SetVector2(const string& name, const Vector2& vt2) {}
	void Setfloat(const string& name, const float f) {}
	void SetUINT(const string& name, const UINT uint) {}
	void SetUINT4(const string& name, const array<UINT, 4>& uint4) {}

	void SetVector3(const string& name, const Vector3& vt3)
	{

	}
	void DrawGui(ComPtr<ID3D11DeviceContext>& context)
	{
		
	}

private:
};

