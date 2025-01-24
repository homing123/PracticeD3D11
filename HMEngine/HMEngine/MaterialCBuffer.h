#pragma once

#include <vector>
#include "D3DUtil.h"
#include "ImGuiUtil.h"
#include <map>
#include <array>

using namespace std;
using Microsoft::WRL::ComPtr;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Vector3;

struct IMaterialCBuffer
{
	virtual void SetVector4(const string& name, const Vector4& vt4) {}
	virtual void SetVector3(const string& name, const Vector3& vt3) {}
	virtual void SetVector2(const string& name, const Vector2& vt2) {}
	virtual void Setfloat(const string& name, const float f) {}
	virtual void SetUINT(const string& name, const UINT uint) {}
	virtual void SetUINT4(const string& name, const array<UINT, 4>& uint4) {}
	virtual void CreateCBuffer(ComPtr<ID3D11Device>& device, ComPtr<ID3D11Buffer>& cbuffer) {};
	virtual void UpdateCBuffer(ComPtr<ID3D11DeviceContext>& context, ComPtr<ID3D11Buffer>& cbuffer) {};
	virtual void DrawGui(ComPtr<ID3D11DeviceContext>& context, ComPtr<ID3D11Buffer> cbuffer) {};
};

__declspec(align(256)) struct BlinnPhongCBuffer : IMaterialCBuffer
{
	const static size_t ambientKey;
	const static size_t shininessKey;
	const static size_t diffuseKey;
	const static size_t specularKey;

	Vector3 ambient;
	float shininess;
	Vector3 diffuse;
	float dummy;
	Vector3 specular;
	float dummy2;
	void Setfloat(const string& name, const float f)
	{
		const size_t key = hash<string>()(name);
		if (key == shininessKey)
		{
			shininess = f;
		}
	}
	void SetVector3(const string& name, const Vector3& vt3)
	{
		const size_t key = hash<string>()(name);
		if (key == ambientKey)
		{
			ambient = vt3;
		}
		else if (key == diffuseKey)
		{
			diffuse = vt3;
		}
		else if (key == specularKey)
		{
			specular = vt3;
		}
	}
	void CreateCBuffer(ComPtr<ID3D11Device>& device, ComPtr<ID3D11Buffer>& cbuffer)
	{
		D3DUtil::CreateCBuffer(device, *this, cbuffer);
	}
	void UpdateCBuffer(ComPtr<ID3D11DeviceContext>& context, ComPtr<ID3D11Buffer>& cbuffer)
	{
		D3DUtil::UpdateCBuffer(context, *this, cbuffer);
	}
	void DrawGui(ComPtr<ID3D11DeviceContext>& context, ComPtr<ID3D11Buffer>& cbuffer)
	{
		ImGui::Text("BlinnPhong");
		ImGui::SliderFloat3("Ambient", &ambient.x, 0, 1);
		ImGui::SliderFloat3("Diffuse", &diffuse.x, 0, 1);
		ImGui::SliderFloat3("Specular", &specular.x, 0, 1);
		ImGui::SliderFloat("Shininess", &shininess, 0, 50);
		D3DUtil::UpdateCBuffer(context, *this, cbuffer);
	}
};
__declspec(align(256)) struct IBLCBuffer : IMaterialCBuffer
{
	const static size_t diffuseKey;
	const static size_t specularKey;

	Vector3 diffuse;
	float dummy;
	Vector3 specular;
	float dummy2;

	void SetVector3(const string& name, const Vector3& vt3)
	{
		const size_t key = hash<string>()(name);
		if (key == diffuseKey)
		{
			diffuse = vt3;
		}
		else if (key == specularKey)
		{
			specular = vt3;
		}
	}
	void CreateCBuffer(ComPtr<ID3D11Device>& device, ComPtr<ID3D11Buffer>& cbuffer)
	{
		D3DUtil::CreateCBuffer(device, *this, cbuffer);
	}
	void UpdateCBuffer(ComPtr<ID3D11DeviceContext>& context, ComPtr<ID3D11Buffer>& cbuffer)
	{
		D3DUtil::UpdateCBuffer(context, *this, cbuffer);
	}
	void DrawGui(ComPtr<ID3D11DeviceContext>& context, ComPtr<ID3D11Buffer>& cbuffer)
	{
		ImGui::Text("IBL");
		ImGui::SliderFloat3("Diffuse", &diffuse.x, 0, 1);
		ImGui::SliderFloat3("Specular", &specular.x, 0, 1);
		D3DUtil::UpdateCBuffer(context, *this, cbuffer);
	}
};

__declspec(align(256)) struct MousePickingCBuffer : IMaterialCBuffer
{
	const static size_t idxKey;

	UINT idx[4] = { 0,0,0,0 };
	void SetUINT4(const string& name, const array<UINT, 4>& uint4)
	{
		const size_t key = hash<string>()(name);
		if (key == idxKey)
		{
			idx[0] = uint4[0];
			idx[1] = uint4[1];
			idx[2] = uint4[2];
			idx[3] = uint4[3];
		}
	}
	void CreateCBuffer(ComPtr<ID3D11Device>& device, ComPtr<ID3D11Buffer>& cbuffer)
	{
		D3DUtil::CreateCBuffer(device, *this, cbuffer);
	}
	void UpdateCBuffer(ComPtr<ID3D11DeviceContext>& context, ComPtr<ID3D11Buffer>& cbuffer)
	{
		D3DUtil::UpdateCBuffer(context, *this, cbuffer);
	}
	void DrawGui(ComPtr<ID3D11DeviceContext>& context, ComPtr<ID3D11Buffer>& cbuffer)
	{

	}
};

__declspec(align(256)) struct SkyboxCBuffer : IMaterialCBuffer
{
	void DrawGui(ComPtr<ID3D11DeviceContext>& context, ComPtr<ID3D11Buffer>& cbuffer)
	{

	}
};

__declspec(align(256)) struct PBRCBuffer : IMaterialCBuffer
{
	int useAlbedo = 0;
	int useNormal = 0;
	int useAO = 0;
	int useMetalic = 0;
	int useRoughness = 0;
	int useEmissive = 0;
	Vector2 dummy;
	void DrawGui(ComPtr<ID3D11DeviceContext>& context, ComPtr<ID3D11Buffer>& cbuffer)
	{

	}
};