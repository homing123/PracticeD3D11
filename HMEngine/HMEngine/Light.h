#pragma once

#include "Transform.h"
#include "ImGuiUtil.h"

class Light
{
public:
	bool m_Active = true;
	Light(ComPtr<ID3D11Device>& device, const Vector3& position, const Vector3& color, const float intensity, const E_LightType type);
	static bool CompareLightType(Light* a, Light* b);
	const E_LightType GetLightType()const;
	void Render(ComPtr<ID3D11DeviceContext>& context);

	Transform* GetPTransform()const;

	virtual void SetLightCBuffer(LightInfo& lightInfo);
	virtual void DrawGui(ComPtr<ID3D11DeviceContext>& context);
public:
	Vector3 m_Color;
	float m_Intensity;
	
protected:
	shared_ptr<Transform> m_Transform;
	ComPtr<ID3D11Buffer> m_VertexBuffer;
	ComPtr<ID3D11Buffer> m_IndexBuffer;
	E_LightType m_LightType;

	const static UINT stride;
	const static UINT offset;
private:
	Light() = default;
};

class DirectionalLight : public Light
{
public:
	DirectionalLight(ComPtr<ID3D11Device>& device, const Vector3& position, const Vector3& euler, const Vector3& color, const float intensity);

	void SetLightCBuffer(LightInfo& lightInfo);
	void DrawGui(ComPtr<ID3D11DeviceContext>& context);
private:
	DirectionalLight() = default;
};
class PointLight : public Light
{
public:
	PointLight(ComPtr<ID3D11Device>& device, const Vector3& position, const Vector3& color, const float intensity, const float fallOffStart, const float fallOffEnd);

public:
	float m_FallOffStart;
	float m_FallOffEnd;
	void SetLightCBuffer(LightInfo& lightInfo);
	void DrawGui(ComPtr<ID3D11DeviceContext>& context);
private:
	PointLight() = default;

};
class SpotLight :public Light
{
public:
	SpotLight(ComPtr<ID3D11Device>& device, const Vector3& position, const Vector3& euler, const Vector3& color, const float intensity, const float fallOffStart, const float fallOffEnd, const float spotPower);

public:
	float m_FallOffStart;
	float m_FallOffEnd;
	float m_SpotPower;
	void SetLightCBuffer(LightInfo& lightInfo);
	void DrawGui(ComPtr<ID3D11DeviceContext>& context);
private:
	SpotLight() = default;

};