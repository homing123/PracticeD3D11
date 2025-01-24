
#include "GameObject.h"

GameObject::GameObject(ComPtr<ID3D11Device>& device, const string& name, Model* pModel, GraphicsPSO* pPSO)
{
	m_Name = name;
	m_Transform = make_shared<Transform>(device);
	m_NameKey = std::hash<string>()(name);
	m_Model = pModel;
	m_PSO = pPSO;
}


Transform* GameObject::GetPTransform() const
{
	return m_Transform.get();
}

void GameObject::Render(ComPtr<ID3D11DeviceContext>& context)
{	
	if (m_Active == true && m_PSO != nullptr && m_Model != nullptr)
	{
		m_Transform->SetTransformCBuffer(context);
		m_PSO->RenderSetting(context);
		for (const auto& mesh : m_Model->m_meshes)
		{
			context->IASetVertexBuffers(0, 1, mesh->vertexBuffer.GetAddressOf(), &mesh->stride, &mesh->offset);
			context->IASetIndexBuffer(mesh->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			context->DrawIndexed(mesh->indexCount, 0, 0);
		}
	}
}

void GameObject::RenderUseCustomPSO(ComPtr<ID3D11DeviceContext>& context)
{
	if (m_Active == true && m_PSO != nullptr && m_Model != nullptr)
	{
		if (m_Transform != nullptr)
		{
			m_Transform->SetTransformCBuffer(context);
		}
		for (const auto& mesh : m_Model->m_meshes)
		{
			context->IASetVertexBuffers(0, 1, mesh->vertexBuffer.GetAddressOf(), &mesh->stride, &mesh->offset);
			context->IASetIndexBuffer(mesh->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			context->DrawIndexed(mesh->indexCount, 0, 0);
		}
	}
}

void GameObject::DrawGui(ComPtr<ID3D11DeviceContext>& context)
{
	ImGui::Text(m_Name.c_str());
	m_Transform->DrawGui();
	m_PSO->m_Material->DrawGui(context);
}