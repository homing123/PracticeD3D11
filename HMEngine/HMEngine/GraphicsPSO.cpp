#include "GraphicsPSO.h"

GraphicsPSO* GraphicsPSO::LastPSO = nullptr;

void GraphicsPSO::SetMatKindAndCreateCBuffer(ComPtr<ID3D11Device>& device, E_MatKind matKind, MaterialCBuffer* pMatCBuffer)
{
	m_MatKind = matKind;
	m_MaterialCBufferCPU = pMatCBuffer;
	switch (m_MatKind)
	{
	case E_MatKind::BlinnPhong:
		D3DUtil::CreateCBuffer(device, *static_cast<BlinnPhongCBuffer*>(m_MaterialCBufferCPU), m_MaterialCBufferGPU);
		break;
	case E_MatKind::IBL:
		D3DUtil::CreateCBuffer(device, *static_cast<IBLCBuffer*>(m_MaterialCBufferCPU), m_MaterialCBufferGPU);
		break;
	case E_MatKind::PBR:
		D3DUtil::CreateCBuffer(device, *static_cast<PBRCBuffer*>(m_MaterialCBufferCPU), m_MaterialCBufferGPU);
		break;
	case E_MatKind::Skybox:
		D3DUtil::CreateCBuffer(device, *static_cast<SkyboxCBuffer*>(m_MaterialCBufferCPU), m_MaterialCBufferGPU);
		break;
	case E_MatKind::MousePicking:
		D3DUtil::CreateCBuffer(device, *static_cast<MousePickingCBuffer*>(m_MaterialCBufferCPU), m_MaterialCBufferGPU);
		break;
	}
}

void GraphicsPSO::RenderSetting(ComPtr<ID3D11DeviceContext>& context)
{
	if (LastPSO != this)
	{
		LastPSO = this;

		context->IASetInputLayout(m_InputLayout.Get());
		context->RSSetState(m_RasterState.Get());
		context->OMSetDepthStencilState(m_DepthStensilState.Get(), m_StencilRef);
		context->IASetPrimitiveTopology(m_PrimitiveTopology);
		context->OMSetBlendState(m_BlendState.Get(), m_BlendFactor, 0xffffffff);
		context->VSSetShader(m_VS.Get(), 0, 0);
		context->HSSetShader(m_HS.Get(), 0, 0);
		context->DSSetShader(m_DS.Get(), 0, 0);
		context->GSSetShader(m_GS.Get(), 0, 0);
		context->PSSetShader(m_PS.Get(), 0, 0);
		context->PSSetConstantBuffers(0, 1, m_MaterialCBufferGPU.GetAddressOf());
	}
}
void GraphicsPSO::UpdateMatCBuffer(ComPtr<ID3D11DeviceContext>& context)
{
	switch (m_MatKind)
	{
	case E_MatKind::BlinnPhong:
		D3DUtil::UpdateCBuffer(context, *static_cast<BlinnPhongCBuffer*>(m_MaterialCBufferCPU), m_MaterialCBufferGPU);
		break;
	case E_MatKind::IBL:
		D3DUtil::UpdateCBuffer(context, *static_cast<IBLCBuffer*>(m_MaterialCBufferCPU), m_MaterialCBufferGPU);
		break;
	case E_MatKind::PBR:
		D3DUtil::UpdateCBuffer(context, *static_cast<PBRCBuffer*>(m_MaterialCBufferCPU), m_MaterialCBufferGPU);
		break;
	case E_MatKind::Skybox:
		D3DUtil::UpdateCBuffer(context, *static_cast<SkyboxCBuffer*>(m_MaterialCBufferCPU), m_MaterialCBufferGPU);
		break;
	case E_MatKind::MousePicking:
		D3DUtil::UpdateCBuffer(context, *static_cast<MousePickingCBuffer*>(m_MaterialCBufferCPU), m_MaterialCBufferGPU);		
		break;
	}
}