#include "Light.h"

Transform* Light::GetPTransform()const { return m_Transform.get(); };

const UINT Light::stride = sizeof(Vector3);
const UINT Light::offset = 0;

Light::Light(ComPtr<ID3D11Device>& device, Vector3& position, Vector3& strength)
{
	m_Transform = make_shared<Transform>(device);
	m_Transform->SetPosition(position);
	m_Strength = strength;

	D3DUtil::CreateBillboardVertexIndexBuffer(device, m_VertexBuffer, m_IndexBuffer);
}
void Light::SetLightCBuffer(LightInfo& lightInfo)
{

}
void Light::Render(ComPtr<ID3D11DeviceContext>& context)
{
	m_Transform->SetTransformCBuffer(context);
	context->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);
	context->Draw(1, 0);
}


DirectionalLight::DirectionalLight(ComPtr<ID3D11Device>& device, Vector3& position, Vector3& euler, Vector3& strength)
	:Light(device, position, strength)
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


PointLight::PointLight(ComPtr<ID3D11Device>& device, Vector3& position, Vector3& strength, float fallOffStart, float fallOffEnd)
	:Light(device, position, strength)
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

SpotLight::SpotLight(ComPtr<ID3D11Device>& device, Vector3& position, Vector3& euler, Vector3& strength, float fallOffStart, float fallOffEnd, float spotPower)
	:Light(device, position, strength)
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