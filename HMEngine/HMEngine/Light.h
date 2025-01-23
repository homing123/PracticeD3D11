#pragma once

#include "Transform.h"

class Light
{
public:
	Light() = default;
	Light(Vector3& position, Vector3& strength);
	Vector3 m_Strength;

	Transform* GetPTransform()const;
	virtual void SetLightCBuffer(LightInfo& lightInfo);

protected:
	shared_ptr<Transform> m_Transform;
};

class DirectionalLight : public Light
{
public:
	DirectionalLight(Vector3& position, Vector3& euler, Vector3& strength);

	void SetLightCBuffer(LightInfo& lightInfo);
};
class PointLight : public Light
{
public:
	PointLight(Vector3& position, Vector3& strength, float fallOffStart, float fallOffEnd);

	float m_FallOffStart;
	float m_FallOffEnd;
	void SetLightCBuffer(LightInfo& lightInfo);
};
class SpotLight :public Light
{
public:
	SpotLight(Vector3& position, Vector3& euler, Vector3& strength, float fallOffStart, float fallOffEnd, float spotPower);

	float m_FallOffStart;
	float m_FallOffEnd;
	float m_SpotPower;
	void SetLightCBuffer(LightInfo& lightInfo);
};