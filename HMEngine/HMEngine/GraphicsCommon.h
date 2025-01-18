#pragma once

#include "GraphicsPSO.h"

namespace Graphics
{
	extern ComPtr<ID3D11SamplerState> LinearWrapSS;
	extern ComPtr<ID3D11SamplerState> LinearClampSS;

	extern ComPtr<ID3D11RasterizerState> SolidCWRS;
	extern ComPtr<ID3D11RasterizerState> SolidCCWRS;
	extern ComPtr<ID3D11RasterizerState> PostProcessingRS;

	extern ComPtr<ID3D11DepthStencilState> BasicDSS;
	extern ComPtr<ID3D11DepthStencilState> SkyboxDSS;

	extern ComPtr<ID3D11InputLayout> BasicMeshInputLayout;
	extern ComPtr<ID3D11InputLayout> SkyboxInputLayout;
	extern ComPtr<ID3D11InputLayout> SamplingInputLayout;

	extern ComPtr<ID3D11VertexShader> BasicMeshVS;
	extern ComPtr<ID3D11VertexShader> SkyboxVS;
	extern ComPtr<ID3D11VertexShader> SamplingVS;

	extern ComPtr<ID3D11PixelShader> BlinnPhongPS;
	extern ComPtr<ID3D11PixelShader> SkyboxPS;

	extern BlinnPhongCBuffer BlinnPhongMatCBuffer_0;
	extern BlinnPhongCBuffer BlinnPhongMatCBuffer_1;
	extern SkyboxCBuffer SkyboxMatCBuffer;

	extern GraphicsPSO BlinnPhongPSO_0;
	extern GraphicsPSO BlinnPhongPSO_1;
	extern GraphicsPSO SkyboxPSO;
	extern GraphicsPSO PBRPSO;

	void InitGraphics(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);
	void InitSampleState(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);
	void InitRasterizerState(ComPtr<ID3D11Device>& device);
	void InitDepthStencilState(ComPtr<ID3D11Device>& device);
	void InitShaders(ComPtr<ID3D11Device>& device);
	void InitMaterial();
	void InitPSO(ComPtr<ID3D11Device>& device);

	void SetSolid(const bool isSolid);
}