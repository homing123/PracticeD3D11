
#include "GraphicsCommon.h"
#include "D3DUtil.h"

namespace Graphics
{
	ComPtr<ID3D11SamplerState> LinearWrapSS;
	ComPtr<ID3D11SamplerState> LinearClampSS;

	ComPtr<ID3D11RasterizerState> SolidCWRS;
	ComPtr<ID3D11RasterizerState> SolidCCWRS;
	ComPtr<ID3D11RasterizerState> PostProcessingRS;

	ComPtr<ID3D11DepthStencilState> BasicDSS;
	ComPtr<ID3D11DepthStencilState> SkyboxDSS;
	ComPtr<ID3D11DepthStencilState> MousePickingDSS;

	ComPtr<ID3D11BlendState> BasicBS;
	ComPtr<ID3D11BlendState> MousePickingBS;

	ComPtr<ID3D11InputLayout> BasicMeshInputLayout;
	ComPtr<ID3D11InputLayout> BillboardPointImageInputLayout;
	ComPtr<ID3D11InputLayout> SkyboxInputLayout;
	ComPtr<ID3D11InputLayout> SamplingInputLayout;

	ComPtr<ID3D11VertexShader> BasicMeshVS;
	ComPtr<ID3D11VertexShader> SkyboxVS;
	ComPtr<ID3D11VertexShader> SamplingVS;
	ComPtr<ID3D11VertexShader> BillboardPointImageVS;

	ComPtr<ID3D11GeometryShader> BillboardPointImageGS;

	ComPtr<ID3D11PixelShader> BlinnPhongPS;
	ComPtr<ID3D11PixelShader> IBLPS;
	ComPtr<ID3D11PixelShader> SkyboxPS;
	ComPtr<ID3D11PixelShader> MousePickingPS;
	ComPtr<ID3D11PixelShader> BillboardPointImagePS;

	BlinnPhongMaterial BlinnPhongMat_0;
	BlinnPhongMaterial BlinnPhongMat_1;
	IBLMaterial IBLMat;
	SkyboxMaterial SkyboxMat;
	MousePickingMaterial MousePickingMat;
	BillboardPointImageMaterial BillboardPointImageMat;

	GraphicsPSO BlinnPhongPSO_0;
	GraphicsPSO BlinnPhongPSO_1;
	GraphicsPSO IBLPSO;
	GraphicsPSO SkyboxPSO;
	GraphicsPSO PBRPSO;
	GraphicsPSO MousePickingPSO;
	GraphicsPSO BillboardPointImagePSO;

}

void Graphics::InitGraphics(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context)
{
	InitSampleState(device, context);
	InitRasterizerState(device);
	InitDepthStencilState(device);
	InitBlendState(device);
	InitShaders(device);
	InitMaterial(device);
	InitPSO(device);
}
void Graphics::InitSampleState(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context)
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	ThrowFail(device->CreateSamplerState(&desc, LinearWrapSS.GetAddressOf()));

	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	ThrowFail(device->CreateSamplerState(&desc, LinearClampSS.GetAddressOf()));

	ID3D11SamplerState* arr_sampler[2] = { LinearWrapSS.Get(), LinearClampSS.Get()};
	context->VSSetSamplers(0, 2, arr_sampler);
	context->PSSetSamplers(0, 2, arr_sampler);
}
void Graphics::InitRasterizerState(ComPtr<ID3D11Device>& device)
{
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FrontCounterClockwise = false;
	desc.DepthClipEnable = true;
	desc.MultisampleEnable = true;
	ThrowFail(device->CreateRasterizerState(&desc, SolidCWRS.GetAddressOf()));

	desc.FrontCounterClockwise = true;
	ThrowFail(device->CreateRasterizerState(&desc, SolidCCWRS.GetAddressOf()));

	desc.CullMode = D3D11_CULL_NONE;
	desc.FrontCounterClockwise = false;
	desc.DepthClipEnable = false;
	ThrowFail(device->CreateRasterizerState(&desc, PostProcessingRS.GetAddressOf()));
}
void Graphics::InitDepthStencilState(ComPtr<ID3D11Device>& device)
{
	D3D11_DEPTH_STENCIL_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	desc.StencilEnable = false;
	ThrowFail(device->CreateDepthStencilState(&desc, BasicDSS.GetAddressOf()));

	ThrowFail(device->CreateDepthStencilState(&desc, MousePickingDSS.GetAddressOf()));

	desc.DepthEnable = false;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	desc.DepthFunc = D3D11_COMPARISON_NEVER;
	ThrowFail(device->CreateDepthStencilState(&desc, SkyboxDSS.GetAddressOf()));
}
void Graphics::InitBlendState(ComPtr<ID3D11Device>& device)
{
	D3D11_RENDER_TARGET_BLEND_DESC rtvDesc;
	D3D11_BLEND_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.BlendEnable = false;
	rtvDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; //기본값 이지만 ZeroMemory 를 했기에 필요
	desc.RenderTarget[0] = rtvDesc;
	device->CreateBlendState(&desc, BasicBS.GetAddressOf());
	device->CreateBlendState(&desc, MousePickingBS.GetAddressOf());
}
void Graphics::InitShaders(ComPtr<ID3D11Device>& device)
{
	vector<D3D11_INPUT_ELEMENT_DESC> vDesc;
	D3D11_INPUT_ELEMENT_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.SemanticName = "POSITION";
	desc.SemanticIndex = 0;
	desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	desc.AlignedByteOffset = 0;
	vDesc.push_back(desc);

	D3DUtil::CreateVertexShaderAndInputLayout(device, L"VSBillboardPointImage.hlsl", vDesc, BillboardPointImageVS, BillboardPointImageInputLayout);

	desc.SemanticName = "NORMAL";
	desc.AlignedByteOffset = 4 * 3;
	vDesc.push_back(desc);

	desc.SemanticName = "TEXCOORD";
	desc.Format = DXGI_FORMAT_R32G32_FLOAT;
	desc.AlignedByteOffset = 4 * 6;
	vDesc.push_back(desc);

	desc.SemanticName = "TANGENT";
	desc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	desc.AlignedByteOffset = 4 * 8;
	vDesc.push_back(desc);

	D3DUtil::CreateVertexShaderAndInputLayout(device, L"VSBasicMesh.hlsl", vDesc, BasicMeshVS, BasicMeshInputLayout);
	D3DUtil::CreateVertexShaderAndInputLayout(device, L"VSSkybox.hlsl", vDesc, SkyboxVS, SkyboxInputLayout);
	
	D3DUtil::CreateGeometryShader(device, L"GSBillboardPointImage.hlsl", BillboardPointImageGS);

	D3DUtil::CreatePixelShader(device, L"PSBlinnPhong.hlsl", BlinnPhongPS);
	D3DUtil::CreatePixelShader(device, L"PSIBL.hlsl", IBLPS);
	D3DUtil::CreatePixelShader(device, L"PSSkybox.hlsl", SkyboxPS);
	D3DUtil::CreatePixelShader(device, L"PSMousePicking.hlsl", MousePickingPS);
	D3DUtil::CreatePixelShader(device, L"PSBillboardPointImage.hlsl", BillboardPointImagePS);
}
void Graphics::InitMaterial(ComPtr<ID3D11Device>& device)
{
	BlinnPhongMat_0.SetVector3("ambient", Vector3(0.2f, 0.1f, 0.1f));
	BlinnPhongMat_0.SetVector3("diffuse", Vector3(0.5f, 0.5f, 0.5f));
	BlinnPhongMat_0.SetVector3("specular", Vector3(0.6f, 0.6f, 0.6f));
	BlinnPhongMat_0.Setfloat("shininess", 15);
	BlinnPhongMat_0.CreateCBuffer(device);
	BlinnPhongMat_1.CreateCBuffer(device);
	IBLMat.CreateCBuffer(device);
	SkyboxMat.CreateCBuffer(device);
	MousePickingMat.CreateCBuffer(device);
	BillboardPointImageMat.CreateCBuffer(device);
}
void Graphics::InitPSO(ComPtr<ID3D11Device>& device)
{
	BlinnPhongPSO_0.m_VS = BasicMeshVS;
	BlinnPhongPSO_0.m_PS = BlinnPhongPS;
	BlinnPhongPSO_0.m_InputLayout = BasicMeshInputLayout;
	BlinnPhongPSO_0.m_RasterState = SolidCWRS;
	BlinnPhongPSO_0.m_DepthStensilState = BasicDSS;
	BlinnPhongPSO_0.m_BlendState = BasicBS;
	BlinnPhongPSO_0.m_Material = &BlinnPhongMat_0;
	
	BlinnPhongPSO_1.m_VS = BasicMeshVS;
	BlinnPhongPSO_1.m_PS = BlinnPhongPS;
	BlinnPhongPSO_1.m_InputLayout = BasicMeshInputLayout;
	BlinnPhongPSO_1.m_RasterState = SolidCWRS;
	BlinnPhongPSO_1.m_DepthStensilState = BasicDSS;
	BlinnPhongPSO_1.m_BlendState = BasicBS;
	BlinnPhongPSO_1.m_Material = &BlinnPhongMat_1;

	IBLPSO.m_VS = BasicMeshVS;
	IBLPSO.m_PS = IBLPS;
	IBLPSO.m_InputLayout = BasicMeshInputLayout;
	IBLPSO.m_RasterState = SolidCWRS;
	IBLPSO.m_DepthStensilState = BasicDSS;
	IBLPSO.m_BlendState = BasicBS;
	IBLPSO.m_Material = &IBLMat;

	SkyboxPSO.m_VS = SkyboxVS;
	SkyboxPSO.m_PS = SkyboxPS;
	SkyboxPSO.m_InputLayout = SkyboxInputLayout;
	SkyboxPSO.m_RasterState = SolidCCWRS;
	SkyboxPSO.m_DepthStensilState = SkyboxDSS;
	SkyboxPSO.m_BlendState = BasicBS;
	SkyboxPSO.m_Material = &SkyboxMat;

	MousePickingPSO.m_VS = BasicMeshVS;
	MousePickingPSO.m_PS = MousePickingPS;
	MousePickingPSO.m_InputLayout = BasicMeshInputLayout;
	MousePickingPSO.m_RasterState = SolidCWRS;
	MousePickingPSO.m_DepthStensilState = MousePickingDSS;
	MousePickingPSO.m_BlendState = MousePickingBS;
	MousePickingPSO.m_Material = &MousePickingMat;

	BillboardPointImagePSO.m_VS = BillboardPointImageVS;
	BillboardPointImagePSO.m_GS = BillboardPointImageGS;
	BillboardPointImagePSO.m_PS = BillboardPointImagePS;
	BillboardPointImagePSO.m_InputLayout = BillboardPointImageInputLayout;
	BillboardPointImagePSO.m_RasterState = SolidCWRS;
	BillboardPointImagePSO.m_DepthStensilState = BasicDSS;
	BillboardPointImagePSO.m_BlendState = BasicBS;
	BillboardPointImagePSO.m_PrimitiveTopology = D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	BillboardPointImagePSO.m_Material = &BillboardPointImageMat;


}