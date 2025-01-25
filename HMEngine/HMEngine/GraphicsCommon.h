#pragma once

#include "GraphicsPSO.h"
#include "BlinnPhongMaterial.h"
#include "IBLMaterial.h"
#include "MousePickingMaterial.h"
#include "SkyboxMaterial.h"
#include "BillboardPointImageMaterial.h"

namespace Graphics
{
	extern ComPtr<ID3D11SamplerState> LinearWrapSS;
	extern ComPtr<ID3D11SamplerState> LinearClampSS;

	extern ComPtr<ID3D11RasterizerState> SolidCWRS;
	extern ComPtr<ID3D11RasterizerState> SolidCCWRS;
	extern ComPtr<ID3D11RasterizerState> PostProcessingRS;

	extern ComPtr<ID3D11DepthStencilState> BasicDSS;
	extern ComPtr<ID3D11DepthStencilState> SkyboxDSS;
	extern ComPtr<ID3D11DepthStencilState> MousePickingDSS;

	extern ComPtr<ID3D11BlendState> BasicBS;
	extern ComPtr<ID3D11BlendState> MousePickingBS;

	extern ComPtr<ID3D11InputLayout> BasicMeshInputLayout;
	extern ComPtr<ID3D11InputLayout> BillboardPointImageInputLayout;
	extern ComPtr<ID3D11InputLayout> SkyboxInputLayout;
	extern ComPtr<ID3D11InputLayout> SamplingInputLayout;

	extern ComPtr<ID3D11VertexShader> BasicMeshVS;
	extern ComPtr<ID3D11VertexShader> SkyboxVS;
	extern ComPtr<ID3D11VertexShader> SamplingVS;
	extern ComPtr<ID3D11VertexShader> BillboardPointImageVS;

	extern ComPtr<ID3D11GeometryShader> BillboardPointImageGS;

	extern ComPtr<ID3D11PixelShader> BlinnPhongPS;
	extern ComPtr<ID3D11PixelShader> IBLPS;
	extern ComPtr<ID3D11PixelShader> SkyboxPS;
	extern ComPtr<ID3D11PixelShader> MousePickingPS;
	extern ComPtr<ID3D11PixelShader> BillboardPointImagePS;

	extern BlinnPhongMaterial BlinnPhongMat_0;
	extern BlinnPhongMaterial BlinnPhongMat_1;
	extern IBLMaterial IBLMat;
	extern SkyboxMaterial SkyboxMat;
	extern MousePickingMaterial MousePickingMat;
	extern BillboardPointImageMaterial BillboardPointImageMat;

	extern GraphicsPSO BlinnPhongPSO_0;
	extern GraphicsPSO BlinnPhongPSO_1;
	extern GraphicsPSO IBLPSO;
	extern GraphicsPSO SkyboxPSO;
	extern GraphicsPSO PBRPSO;
	extern GraphicsPSO MousePickingPSO;
	extern GraphicsPSO BillboardPointImagePSO;

	void InitGraphics(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);
	void InitSampleState(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context);
	void InitRasterizerState(ComPtr<ID3D11Device>& device);
	void InitDepthStencilState(ComPtr<ID3D11Device>& device);
	void InitBlendState(ComPtr<ID3D11Device>& device);
	void InitShaders(ComPtr<ID3D11Device>& device);
	void InitMaterial(ComPtr<ID3D11Device>& device);
	void InitPSO(ComPtr<ID3D11Device>& device);

	void SetSolid(const bool isSolid);
}