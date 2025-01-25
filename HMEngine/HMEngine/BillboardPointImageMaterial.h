#pragma once
#include "Material.h"
#include "D3DUtil.h"

__declspec(align(256)) struct BillboardPointImageCBuffer
{
	UINT idx[4] = { 0,0,0,0 };
};


class BillboardPointImageMaterial : public Material
{
public:
	const static size_t idxKey;
	void CreateCBuffer(ComPtr<ID3D11Device>& device)
	{
		D3DUtil::CreateCBuffer(device, m_MatCBufferCPU, m_MatCBufferGPU);
	}
	void UpdateCBuffer(ComPtr<ID3D11DeviceContext>& context)
	{
		D3DUtil::UpdateCBuffer(context, m_MatCBufferCPU, m_MatCBufferGPU);
	}
	void SetVector4(const string& name, const Vector4& vt4) {}
	void SetVector3(const string& name, const Vector3& vt3) {}
	void SetVector2(const string& name, const Vector2& vt2) {}
	void SetUINT(const string& name, const UINT uint) {}
	void Setfloat(const string& name, const float f) {}

	void SetUINT4(const string& name, const array<UINT, 4>& uint4)
	{
		const size_t key = hash<string>()(name);
		if (key == idxKey)
		{
			isChanged = true;
			m_MatCBufferCPU.idx[0] = uint4[0];
			m_MatCBufferCPU.idx[1] = uint4[1];
			m_MatCBufferCPU.idx[2] = uint4[2];
			m_MatCBufferCPU.idx[3] = uint4[3];
		}
	}
	void DrawGui(ComPtr<ID3D11DeviceContext>& context) {}

private:
	BillboardPointImageCBuffer m_MatCBufferCPU;
};

