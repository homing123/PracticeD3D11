//#include "CubeMapping.h"
//const size_t CubeMapping::MeshKey = hash<string>()("CubeMap");
//
//void CubeMapping::Init(ID3D11Device* pDevice)
//{
//	ComPtr<ID3D11Texture2D> texture;
//	const wstring diffuseMapPath = L"C:/Users/k9996/Desktop/DX/Models/CubemapTextures/Atrium_diffuseIBL.dds";
//	const wstring specularMapPath = L"C:/Users/k9996/Desktop/DX/Models/CubemapTextures/Atrium_specularIBL.dds";
//	HRESULT hr = CreateDDSTextureFromFileEx(pDevice, diffuseMapPath.c_str(), 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, D3D11_RESOURCE_MISC_TEXTURECUBE, DDS_LOADER_FLAGS(false),
//		(ID3D11Resource**)texture.GetAddressOf(), m_DiffuseResourceView.GetAddressOf(), nullptr);
//
//	if (FAILED(hr))
//	{
//		cout << "CreateDDSTextureFrimFileEx failed" << endl;
//	}
//
//	hr = CreateDDSTextureFromFileEx(pDevice, specularMapPath.c_str(), 0, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, D3D11_RESOURCE_MISC_TEXTURECUBE, DDS_LOADER_FLAGS(false),
//		(ID3D11Resource**)texture.GetAddressOf(), m_SpecularResourceView.GetAddressOf(), nullptr);
//
//	if (FAILED(hr))
//	{
//		cout << "CreateDDSTextureFrimFileEx failed" << endl;
//	}
//
//	VSCBuffer vsCBuffer;
//	PSCBuffer psCBuffer;
//
//	D3DUtil::CreateCBuffer(pDevice, vsCBuffer, m_VSCBuffer);
//	D3DUtil::CreateCBuffer(pDevice, psCBuffer, m_PSCBuffer);
//
//	vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc = {
//		{"POSITION",0 , DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
//		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
//		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0},
//	};
//	D3DUtil::CreateVertexShaderAndInputLayout(pDevice, L"VSCubeMap.hlsl", inputElementDesc, m_VS, m_InputLayout);
//	D3DUtil::CreatePixelShader(pDevice, L"PSCubeMap.hlsl", m_PS);
//	
//}
//
//void CubeMapping::UpdateVSCBuffer(ID3D11DeviceContext* pContext, const Matrix& MatView, const Matrix& MatProj)
//{
//	VSCBuffer vsCBuffer;
//	vsCBuffer.Identity();
//	vsCBuffer.view = MatView;
//	vsCBuffer.proj = MatProj;
//	D3DUtil::UpdateCBuffer(pContext, vsCBuffer, m_VSCBuffer);
//}
//
