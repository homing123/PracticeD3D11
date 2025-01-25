#pragma once

#include <vector>
#include "D3DUtil.h"
#include "ImGuiUtil.h"
#include <array>

using namespace std;
using Microsoft::WRL::ComPtr;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Vector3;

enum E_MatKind
{
	PBR,
	BlinnPhong,
	IBL,
	Skybox,
	MousePicking,
};

class Material
{
public:
	virtual void CreateCBuffer(ComPtr<ID3D11Device>& device) = 0;
	virtual void SetVector4(const string& name, const Vector4& vt4) = 0;
	virtual void SetVector3(const string& name, const Vector3& vt3) = 0;
	virtual void SetVector2(const string& name, const Vector2& vt2) = 0;
	virtual void Setfloat(const string& name, const float f) = 0;
	virtual void SetUINT(const string& name, const UINT uint) = 0;
	virtual void SetUINT4(const string& name, const array<UINT,4>& uint4) = 0;
	virtual void DrawGui(ComPtr<ID3D11DeviceContext>& context) = 0;
	virtual void UpdateCBuffer(ComPtr<ID3D11DeviceContext>& context) = 0;

	void PSSetCBuffer(ComPtr<ID3D11DeviceContext>& context)
	{
		if (isChanged)
		{
			UpdateCBuffer(context);
		}
		context->PSSetConstantBuffers(0, 1, m_MatCBufferGPU.GetAddressOf());
	}
protected:
	bool isChanged = false;
	E_MatKind m_MatKind;
	ComPtr<ID3D11Buffer> m_MatCBufferGPU;

private:

};
