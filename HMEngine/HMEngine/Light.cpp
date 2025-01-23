#include "Light.h"

Transform* Light::GetPTransform()const { return m_Transform.get(); };

Light::Light(Vector3& position, Vector3& strength)
{
	m_Transform = make_shared<Transform>();
	m_Transform->SetPosition(position);
	m_Strength = strength;
}
void Light::SetLightCBuffer(LightInfo& lightInfo)
{

}


DirectionalLight::DirectionalLight(Vector3& position, Vector3& euler, Vector3& strength)
	:Light(position, strength)
{
	m_Transform->SetEuler(euler);
}
void DirectionalLight::SetLightCBuffer(LightInfo& lightInfo)
{
	//Light::SetLightCBuffer(cbuffer); //이걸로 부모함수 호출 가능

	lightInfo.LightKind = E_LightKind::Directional;
	lightInfo.strength = m_Strength;
	lightInfo.direction = m_Transform->GetForward();
}


PointLight::PointLight(Vector3& position, Vector3& strength, float fallOffStart, float fallOffEnd)
	:Light(position, strength)
{
	m_FallOffStart = fallOffStart;
	m_FallOffEnd = fallOffEnd;
}
void PointLight::SetLightCBuffer(LightInfo& lightInfo)
{
	lightInfo.LightKind = E_LightKind::Point;
	lightInfo.strength = m_Strength;
	lightInfo.fallOffStart = m_FallOffStart;
	lightInfo.fallOffEnd = m_FallOffEnd;
	lightInfo.position = m_Transform->GetPosition();
}

SpotLight::SpotLight(Vector3& position, Vector3& euler, Vector3& strength, float fallOffStart, float fallOffEnd, float spotPower)
	:Light(position, strength)
{
	m_Transform->SetEuler(euler);
	m_FallOffStart = fallOffStart;
	m_FallOffEnd = fallOffEnd;
	m_SpotPower = spotPower;
}

void SpotLight::SetLightCBuffer(LightInfo& lightInfo)
{
	lightInfo.LightKind = E_LightKind::Spot;
	lightInfo.strength = m_Strength;
	lightInfo.fallOffStart = m_FallOffStart;
	lightInfo.fallOffEnd = m_FallOffEnd;
	lightInfo.position = m_Transform->GetPosition();
	lightInfo.direction = m_Transform->GetForward();
	lightInfo.spotPower = m_SpotPower;
}