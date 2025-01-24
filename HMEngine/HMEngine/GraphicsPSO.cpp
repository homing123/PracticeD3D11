#include "GraphicsPSO.h"

GraphicsPSO* GraphicsPSO::LastPSO = nullptr;

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
		m_Material->PSSetCBuffer(context);
	}
}