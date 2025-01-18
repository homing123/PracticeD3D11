//#include "ImageFilter.h"
//wstring ImageFilter::VSName = L"VSImageFilter.hlsl";
//string ImageFilter::MeshName = "ImageFilterSquare";
//void ImageFilter::Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Mesh* pMesh, const wstring& psName, const int width, const int height)
//{
//	m_Mesh = pMesh;
//
//	vector<D3D11_INPUT_ELEMENT_DESC> inputElementDesc = {
//		{"POSITION",0 , DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
//		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0},
//		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0},
//	};
//
//	D3DUtil::CreateVertexShaderAndInputLayout(pDevice, VSName, inputElementDesc, m_VS, m_InputLayout);
//	D3DUtil::CreatePixelShader(pDevice, psName, m_PS);
//
//	D3D11_SAMPLER_DESC sampDesc;
//	ZeroMemory(&sampDesc, sizeof(sampDesc));
//	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
//	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
//	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
//	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
//	sampDesc.MinLOD = 0;
//	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
//
//	HRESULT hr = pDevice->CreateSamplerState(&sampDesc, m_SamplerState.GetAddressOf());
//	if (FAILED(hr)) {
//		cout << "CreateSamplerState failed" << endl;
//	}
//
//	D3D11_RASTERIZER_DESC rastDesc;
//	ZeroMemory(&rastDesc, sizeof(rastDesc));
//	rastDesc.FillMode = D3D11_FILL_SOLID;
//	rastDesc.CullMode = D3D11_CULL_NONE;
//	rastDesc.FrontCounterClockwise = false;
//	rastDesc.DepthClipEnable = false;
//
//	pDevice->CreateRasterizerState(&rastDesc, m_RasterizerState.GetAddressOf());
//
//	ZeroMemory(&m_Viewport, sizeof(m_Viewport));
//	m_Viewport.TopLeftX = 0;
//	m_Viewport.TopLeftY = 0;
//	m_Viewport.Width = (float)width;
//	m_Viewport.Height = (float)height;
//	m_Viewport.MinDepth = 0.0f;
//	m_Viewport.MaxDepth = 1.0f;
//
//	ComPtr<ID3D11Texture2D> tex;
//	D3D11_TEXTURE2D_DESC texDesc;
//	ZeroMemory(&texDesc, sizeof(texDesc));
//	texDesc.Width = width;
//	texDesc.Height = height;
//	texDesc.MipLevels = texDesc.ArraySize = 1;
//	texDesc.SampleDesc.Count = 1;
//	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//	texDesc.Usage = D3D11_USAGE_DEFAULT;
//	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS;
//	texDesc.MiscFlags = 0;
//	texDesc.CPUAccessFlags = 0;
//
//	D3D11_RENDER_TARGET_VIEW_DESC viewDesc;
//	ZeroMemory(&viewDesc, sizeof(viewDesc));
//	viewDesc.Format = texDesc.Format;
//	viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
//	viewDesc.Texture2D.MipSlice = 0;
//
//	hr = pDevice->CreateTexture2D(&texDesc, NULL, tex.GetAddressOf());
//	if (FAILED(hr))
//	{
//		cout << "CreateTexture2D failed" << endl;
//	}
//	hr = pDevice->CreateRenderTargetView(tex.Get(), &viewDesc, m_RenderTargetView.GetAddressOf());
//	if (FAILED(hr))
//	{
//		cout << "CreateRenderTargetView failed" << endl;
//	}
//	hr = pDevice->CreateShaderResourceView(tex.Get(), nullptr, m_ShaderResourceView.GetAddressOf());
//	if (FAILED(hr))
//	{
//		cout << "CreateShaderResourceView failed" << endl;
//	}
//
//	m_CBufferCPU.dx = 1.0f / width;
//	m_CBufferCPU.dy = 1.0f / height;
//
//	D3DUtil::CreateCBuffer(pDevice, m_CBufferCPU, m_CBufferGPU);
//	SetRenderTargets({ m_RenderTargetView });
//}
//
//void ImageFilter::SetShaderResources(const std::vector<ComPtr<ID3D11ShaderResourceView>>& resources)
//{
//	m_ShaderResourceViews.clear();
//	for (const auto& res : resources)
//	{
//		m_ShaderResourceViews.push_back(res.Get());
//	}
//}
//void ImageFilter::SetRenderTargets(const vector<ComPtr<ID3D11RenderTargetView>>& renderTargets)
//{
//	m_RenderTargets.clear();
//	for (const auto& target : renderTargets)
//	{
//		m_RenderTargets.push_back(target.Get());
//	}
//}
//void ImageFilter::UpdateCBuffer(ID3D11DeviceContext* pContext)
//{
//	D3DUtil::UpdateCBuffer(pContext, m_CBufferCPU, m_CBufferGPU);
//}
//
//void ImageFilter::Render(ID3D11DeviceContext* pContext)
//{
//	assert(m_ShaderResourceViews.size() > 0);
//	assert(m_RenderTargets.size() > 0);
//	ID3D11RenderTargetView* point = m_RenderTargetView.Get();
//
//	pContext->OMSetRenderTargets(UINT(m_RenderTargets.size()), m_RenderTargets.data(), nullptr);
//	pContext->RSSetState(m_RasterizerState.Get());
//	pContext->RSSetViewports(1, &m_Viewport);
//
//	UINT stride = sizeof(Vertex);
//	UINT offset = 0;
//	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	pContext->IASetInputLayout(m_InputLayout.Get());
//	pContext->IASetVertexBuffers(0, 1, m_Mesh->m_VertexBuffer.GetAddressOf(), &stride, &offset);
//	pContext->IASetIndexBuffer(m_Mesh->m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
//
//	pContext->VSSetShader(m_VS.Get(), 0, 0);
//	pContext->PSSetShader(m_PS.Get(), 0, 0);
//
//	pContext->PSSetConstantBuffers(0, 1, m_CBufferGPU.GetAddressOf());
//	
//	pContext->PSSetShaderResources(0, UINT(m_ShaderResourceViews.size()), m_ShaderResourceViews.data());
//	pContext->PSSetSamplers(0, 1, m_SamplerState.GetAddressOf());
//	pContext->DrawIndexed(m_Mesh->GetIndexCount(), 0, 0); 
//
//}