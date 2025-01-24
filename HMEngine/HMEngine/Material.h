#pragma once

#include <vector>
#include "D3DUtil.h"
#include "ImGuiUtil.h"
#include "MaterialCBuffer.h"
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
	E_MatKind m_MatKind;
	IMaterialCBuffer* m_MatCBufferCPU = nullptr;
	ComPtr<ID3D11Buffer> m_MatCBufferGPU;

	void CreateMaterialCBufferCPU(ComPtr<ID3D11Device>& device, E_MatKind matKind)
	{
		m_MatKind = matKind;
		switch (matKind)
		{
		case E_MatKind::BlinnPhong:
			m_MatCBufferCPU = new BlinnPhongCBuffer();
			break;
		case E_MatKind::IBL:
			m_MatCBufferCPU = new IBLCBuffer();
			break;
		case E_MatKind::MousePicking:
			m_MatCBufferCPU = new MousePickingCBuffer();
			break;
		case E_MatKind::Skybox:
			m_MatCBufferCPU = new SkyboxCBuffer();
			break;
		}
		m_MatCBufferCPU->CreateCBuffer(device, m_MatCBufferGPU);
	}
	void SetVector4(const string& name, const Vector4& vt4)
	{
		isChanged = true;
		m_MatCBufferCPU->SetVector4(name, vt4);
	}
	void SetVector3(const string& name, const Vector3& vt3)
	{
		isChanged = true;
		m_MatCBufferCPU->SetVector3(name, vt3);
	}
	void SetVector2(const string& name, const Vector2& vt2)
	{
		isChanged = true;
		m_MatCBufferCPU->SetVector2(name, vt2);
	}
	void Setfloat(const string& name, const float f)
	{
		isChanged = true;
		m_MatCBufferCPU->Setfloat(name, f);
	}
	void SetUINT(const string& name, const UINT uint)
	{
		isChanged = true;
		m_MatCBufferCPU->SetUINT(name, uint);
	}
	void SetUINT4(const string& name, const array<UINT,4> uint4)
	{
		isChanged = true;
		m_MatCBufferCPU->SetUINT4(name, uint4);
	}

	void PSSetCBuffer(ComPtr<ID3D11DeviceContext>& context)
	{
		if (isChanged)
		{
			m_MatCBufferCPU->UpdateCBuffer(context, m_MatCBufferGPU);
		}
		context->PSSetConstantBuffers(0, 1, m_MatCBufferGPU.GetAddressOf());
	}

	void DrawGui(ComPtr<ID3D11DeviceContext>& context)
	{
		m_MatCBufferCPU->DrawGui(context, m_MatCBufferGPU);
	}
protected:
private:
	bool isChanged = false;

};
