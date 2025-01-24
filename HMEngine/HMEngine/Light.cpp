#include "Light.h"

Transform* Light::GetPTransform()const { return m_Transform.get(); };

const UINT Light::stride = sizeof(Vector3);
const UINT Light::offset = 0;


Light::Light(ComPtr<ID3D11Device>& device, Vector3& position, Vector3& strength, const E_LightType type)
{
	m_Transform = make_shared<Transform>(device);
	m_Transform->SetPosition(position);
	m_Strength = strength;
	m_LightType = type;

	D3DUtil::CreateBillboardVertexIndexBuffer(device, m_VertexBuffer, m_IndexBuffer);
}
const E_LightType Light::GetLightType() const
{
	return m_LightType;
}	
bool Light::CompareLightType(Light* a, Light* b)
{
	return a->m_LightType < b->m_LightType; //static_cast<int>(a->m_LightType) < static_cast<int>(b->m_LightType);
}
void Light::Render(ComPtr<ID3D11DeviceContext>& context)
{
	m_Transform->SetTransformCBuffer(context);
	context->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
	context->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);
	context->Draw(1, 0);
}


void Light::SetLightCBuffer(LightInfo& lightInfo) {}
void Light::DrawGui(ComPtr<ID3D11DeviceContext>& context) {}



DirectionalLight::DirectionalLight(ComPtr<ID3D11Device>& device, Vector3 position, Vector3 euler, Vector3 strength)
	:Light(device, position, strength, E_LightType::Directional)
{
	m_Transform->SetEuler(euler);
}
void DirectionalLight::SetLightCBuffer(LightInfo& lightInfo)
{
	//Light::SetLightCBuffer(cbuffer); //이걸로 부모함수 호출 가능

	lightInfo.LightType = E_LightType::Directional;
	lightInfo.strength = m_Strength;
	lightInfo.direction = m_Transform->GetForward();
}
void DirectionalLight::DrawGui(ComPtr<ID3D11DeviceContext>& context)
{
	ImGui::Text("Directional Light");
	m_Transform->DrawGui();
	ImGui::DragFloat("Strength", &m_Strength.x, 0.05f, 0.0f);
}




PointLight::PointLight(ComPtr<ID3D11Device>& device, Vector3 position, Vector3 strength, float fallOffStart, float fallOffEnd)
	:Light(device, position, strength, E_LightType::Point)
{
	m_FallOffStart = fallOffStart;
	m_FallOffEnd = fallOffEnd;
}
void PointLight::SetLightCBuffer(LightInfo& lightInfo)
{
	lightInfo.LightType = E_LightType::Point;
	lightInfo.strength = m_Strength;
	lightInfo.fallOffStart = m_FallOffStart;
	lightInfo.fallOffEnd = m_FallOffEnd;
	lightInfo.position = m_Transform->GetPosition();
}
void PointLight::DrawGui(ComPtr<ID3D11DeviceContext>& context)
{
	ImGui::Text("Point Light");
	m_Transform->DrawGui();
	ImGui::DragFloat("Strength", &m_Strength.x, 0.05f, 0.0f);
	ImGui::DragFloat("FallOffStart", &m_FallOffStart, 0.05f, 0.0f);
	ImGui::DragFloat("FallOffEnd", &m_FallOffEnd, 0.05f, 0.0f);
}




SpotLight::SpotLight(ComPtr<ID3D11Device>& device, Vector3 position, Vector3 euler, Vector3 strength, float fallOffStart, float fallOffEnd, float spotPower)
	:Light(device, position, strength, E_LightType::Spot)
{
	m_Transform->SetEuler(euler);
	m_FallOffStart = fallOffStart;
	m_FallOffEnd = fallOffEnd;
	m_SpotPower = spotPower;
}

void SpotLight::SetLightCBuffer(LightInfo& lightInfo)
{
	lightInfo.LightType = E_LightType::Spot;
	lightInfo.strength = m_Strength;
	lightInfo.fallOffStart = m_FallOffStart;
	lightInfo.fallOffEnd = m_FallOffEnd;
	lightInfo.position = m_Transform->GetPosition();
	lightInfo.direction = m_Transform->GetForward();
	lightInfo.spotPower = m_SpotPower;
}
void SpotLight::DrawGui(ComPtr<ID3D11DeviceContext>& context)
{
	ImGui::Text("Point Light");
	m_Transform->DrawGui();
	ImGui::DragFloat("Strength", &m_Strength.x, 0.05f, 0.0f);
	ImGui::DragFloat("FallOffStart", &m_FallOffStart, 0.05f, 0.0f);
	ImGui::DragFloat("FallOffEnd", &m_FallOffEnd, 0.05f, 0.0f);
	ImGui::DragFloat("SpotPower", &m_SpotPower, 0.05f, 0.0f);
}