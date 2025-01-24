
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
//void GameObject::UpdateVSCBuffer(ID3D11DeviceContext* pContext, const Matrix& MatView, const Matrix& MatProj)
//{
//	VSCBuffer vsCBuffer;
//	Transform* pGOTransform = GetPTransform();
//	Matrix model = pGOTransform->GetModelMat();
//	vsCBuffer.model = model.Transpose();
//	vsCBuffer.invT = model;
//	vsCBuffer.invT.Translation(Vector3(0.f));
//	vsCBuffer.invT = vsCBuffer.invT.Invert();
//
//	vsCBuffer.view = MatView;
//	vsCBuffer.proj = MatProj;
//
//	D3DUtil::UpdateCBuffer(pContext, vsCBuffer, m_VSCBuffer);
//}
//void GameObject::UpdatePSCBuffer(ID3D11DeviceContext* pContext, const PSCBuffer& psCBuffer)
//{
//	D3DUtil::UpdateCBuffer(pContext, psCBuffer, m_PSCBuffer);
//}

//void GameObject::Render(ID3D11DeviceContext* pContext, const Matrix& MatView, const Matrix& MatProj, PSCBuffer& psCBuffer, ID3D11ShaderResourceView*const* texViews, ID3D11SamplerState*const* samplers)
//{
//	UINT stride = sizeof(Vertex);
//	UINT offset = 0;
//
//	pContext->IASetInputLayout(m_Material->m_InputLayout.Get());
//	pContext->IASetVertexBuffers(0, 1, m_Mesh->m_VertexBuffer.GetAddressOf(), &stride, &offset);
//	pContext->IASetIndexBuffer(m_Mesh->m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
//
//	pContext->VSSetShader(m_Material->m_VS.Get(), 0, 0);
//	pContext->PSSetShader(m_Material->m_PS.Get(), 0, 0);
//	UpdateVSCBuffer(pContext, MatView, MatProj);
//	UpdatePSCBuffer(pContext, psCBuffer);
//	pContext->PSSetShaderResources(0, 3, texViews);
//	pContext->PSSetSamplers(0, 1, samplers);
//	pContext->VSSetConstantBuffers(0, 1, m_VSCBuffer.GetAddressOf());
//	pContext->PSSetConstantBuffers(0, 1, m_PSCBuffer.GetAddressOf());
//
//	pContext->DrawIndexed(m_Mesh->GetIndexCount(), 0, 0);
//}