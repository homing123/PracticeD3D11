#pragma once

#include "Transform.h"

class Light
{
public:
	Light() = default;
	Light(ComPtr<ID3D11Device>& device, Vector3& position, Vector3& strength, const E_LightType type);
	Vector3 m_Strength;
	const E_LightType GetLightType()const;
	Transform* GetPTransform()const;
	virtual void SetLightCBuffer(LightInfo& lightInfo);
	void Render(ComPtr<ID3D11DeviceContext>& context);

	static bool CompareLightType(Light* a, Light* b);
protected:
	shared_ptr<Transform> m_Transform;
	ComPtr<ID3D11Buffer> m_VertexBuffer;
	ComPtr<ID3D11Buffer> m_IndexBuffer;
	E_LightType m_LightType;

	const static UINT stride;
	const static UINT offset;
};

class DirectionalLight : public Light
{
public:
	DirectionalLight(ComPtr<ID3D11Device>& device, Vector3 position, Vector3 euler, Vector3 strength);

	void SetLightCBuffer(LightInfo& lightInfo);
private:
	DirectionalLight() = default;
};
class PointLight : public Light
{
public:
	PointLight(ComPtr<ID3D11Device>& device, Vector3 position, Vector3 strength, float fallOffStart, float fallOffEnd);

	float m_FallOffStart;
	float m_FallOffEnd;
	void SetLightCBuffer(LightInfo& lightInfo);
private:
	PointLight() = default;

};
class SpotLight :public Light
{
public:
	SpotLight(ComPtr<ID3D11Device>& device, Vector3 position, Vector3 euler, Vector3 strength, float fallOffStart, float fallOffEnd, float spotPower);

	float m_FallOffStart;
	float m_FallOffEnd;
	float m_SpotPower;
	void SetLightCBuffer(LightInfo& lightInfo);
private:
	SpotLight() = default;

};