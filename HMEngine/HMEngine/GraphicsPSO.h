#pragma once

#include "Material.h"
#include "D3DUtil.h"


class GraphicsPSO
{
private:
	static GraphicsPSO* LastPSO;
public:

	//shader
	ComPtr<ID3D11VertexShader> m_VS;
	ComPtr<ID3D11HullShader> m_HS;
	ComPtr<ID3D11DomainShader> m_DS;
	ComPtr<ID3D11GeometryShader> m_GS;
	ComPtr<ID3D11PixelShader> m_PS;

	ComPtr<ID3D11InputLayout> m_InputLayout;
	ComPtr<ID3D11RasterizerState> m_RasterState;
	ComPtr<ID3D11DepthStencilState> m_DepthStensilState;
	ComPtr<ID3D11BlendState> m_BlendState;
	
	float m_BlendFactor[4] = { 0,0,0,1 };
	UINT m_StencilRef = 0;

	D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	Material* m_Material;

public:
	void RenderSetting(ComPtr<ID3D11DeviceContext>& context);

};