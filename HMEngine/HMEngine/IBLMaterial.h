#pragma once
#include "Material.h"
#include "D3DUtil.h"

__declspec(align(256)) struct IBLCBuffer
{
	Vector3 diffuse;
	float dummy;
	Vector3 specular;
	float dummy2;
};


class IBLMaterial : public Material
{
public:
	const static size_t diffuseKey;
	const static size_t specularKey;
	void CreateCBuffer(ComPtr<ID3D11Device>& device)
	{
		D3DUtil::CreateCBuffer(device, m_MatCBufferCPU, m_MatCBufferGPU);
	}
	void UpdateCBuffer(ComPtr<ID3D11DeviceContext>& context)
	{
		D3DUtil::UpdateCBuffer(context, m_MatCBufferCPU, m_MatCBufferGPU);
	}
	void SetVector4(const string& name, const Vector4& vt4) {}
	void SetVector2(const string& name, const Vector2& vt2) {}
	void Setfloat(const string& name, const float f) {}
	void SetUINT(const string& name, const UINT uint) {}
	void SetUINT4(const string& name, const array<UINT, 4>& uint4) {}

	void SetVector3(const string& name, const Vector3& vt3)
	{
		const size_t key = hash<string>()(name);
		if (key == diffuseKey)
		{
			isChanged = true;
			m_MatCBufferCPU.diffuse = vt3;
		}
		else if (key == specularKey)
		{
			isChanged = true;
			m_MatCBufferCPU.specular = vt3;
		}
	}
	void DrawGui(ComPtr<ID3D11DeviceContext>& context)
	{
		ImGui::Text("IBL");
		ImGui::SliderFloat3("Diffuse", &m_MatCBufferCPU.diffuse.x, 0, 1);
		ImGui::SliderFloat3("Specular", &m_MatCBufferCPU.specular.x, 0, 1);
		D3DUtil::UpdateCBuffer(context, m_MatCBufferCPU, m_MatCBufferGPU);
	}

private:
	IBLCBuffer m_MatCBufferCPU;
};

