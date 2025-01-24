#pragma once

#include "Transform.h"

class Light
{
public:
	Light() = default;
	Light(ComPtr<ID3D11Device>& device, Vector3& position, Vector3& strength);
	Vector3 m_Strength;

	Transform* GetPTransform()const;
	virtual void SetLightCBuffer(LightInfo& lightInfo);
	void Render(ComPtr<ID3D11DeviceContext>& context);

protected:
	shared_ptr<Transform> m_Transform;
	ComPtr<ID3D11Buffer> m_VertexBuffer;
	ComPtr<ID3D11Buffer> m_IndexBuffer;

	const static UINT stride;
	const static UINT offset;
};

class DirectionalLight : public Light
{
public:
	DirectionalLight(ComPtr<ID3D11Device>& device, Vector3& position, Vector3& euler, Vector3& strength);

	void SetLightCBuffer(LightInfo& lightInfo);
};
class PointLight : public Light
{
public:
	PointLight(ComPtr<ID3D11Device>& device, Vector3& position, Vector3& strength, float fallOffStart, float fallOffEnd);

	float m_FallOffStart;
	float m_FallOffEnd;
	void SetLightCBuffer(LightInfo& lightInfo);
};
class SpotLight :public Light
{
public:
	SpotLight(ComPtr<ID3D11Device>& device, Vector3& position, Vector3& euler, Vector3& strength, float fallOffStart, float fallOffEnd, float spotPower);

	float m_FallOffStart;
	float m_FallOffEnd;
	float m_SpotPower;
	void SetLightCBuffer(LightInfo& lightInfo);
};