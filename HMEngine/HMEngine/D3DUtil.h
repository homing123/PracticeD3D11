#pragma once

#include <wrl.h>
#include <iostream>
#include <vector>
#include <stb_image.h>

#pragma comment(lib, "d3d11.lib")
#include <d3dcommon.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxtk/DDSTextureLoader.h>

#include "Vertex.h"

using namespace std;
using Microsoft::WRL::ComPtr;
using namespace DirectX;

inline void ThrowFail(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw std::exception();
	}
}

class D3DUtil
{
public:
	template<typename T>
	static void CreateCBuffer(ComPtr<ID3D11Device>& device, const T& cBufferData, ComPtr<ID3D11Buffer>& cBuffer)
	{
		D3D11_BUFFER_DESC CBufferDesc;
		CBufferDesc.ByteWidth = sizeof(cBufferData);
		CBufferDesc.Usage = D3D11_USAGE_DYNAMIC; //CPU 쓰기, GPU 읽기
		CBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		CBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		CBufferDesc.MiscFlags = 0;
		CBufferDesc.StructureByteStride = 0; //constantbuffer에서는 미사용

		D3D11_SUBRESOURCE_DATA CBufferResourceData;
		CBufferResourceData.pSysMem = &cBufferData;
		CBufferResourceData.SysMemPitch = 0;
		CBufferResourceData.SysMemSlicePitch = 0;

		HRESULT hResult = device->CreateBuffer(&CBufferDesc, &CBufferResourceData, cBuffer.GetAddressOf());
		if (FAILED(hResult))
		{
			cout << " Create CBuffer Failed" << endl;
			return;
		}
	}
	template<typename T>
	static void UpdateCBuffer(ComPtr<ID3D11DeviceContext>& context, const T& cBufferData, ComPtr<ID3D11Buffer>& cBuffer)
	{
		D3D11_MAPPED_SUBRESOURCE ms;
		HRESULT hr = context->Map(cBuffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		if (FAILED(hr))
		{
			cout << "UpdateCBuffer map failed" << endl;
		}
		memcpy(ms.pData, &cBufferData, sizeof(cBufferData));
		context->Unmap(cBuffer.Get(), NULL);
		if (FAILED(hr))
		{
			cout << "UpdateCBuffer unmap failed" << endl;
		}
	}
	static void CreateVertexBuffer(ComPtr<ID3D11Device>& device, const vector<Vertex>& hBuffer, ComPtr<ID3D11Buffer>& dBuffer);
	static void CreateIndexBuffer(ComPtr<ID3D11Device>& device, const vector<uint32_t>& hBuffer, ComPtr<ID3D11Buffer>& dBuffer);
	static bool CreateRenderTargetView(ComPtr<ID3D11Device>& device, ComPtr<IDXGISwapChain>& swapChain, ComPtr<ID3D11RenderTargetView>& renderTargetView);
	static bool CreateRenderTargetShaderResourceView(ComPtr<ID3D11Device>& device, ComPtr<IDXGISwapChain>& swapChain, ComPtr<ID3D11RenderTargetView>& renderTargetView, ComPtr<ID3D11ShaderResourceView>& shaderResourceView);
	static bool CreateDepthBuffer(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DepthStencilView>& depthStencilView, const UINT width, const UINT height, const int qualitylevels);
	static void SetViewport(ComPtr<ID3D11DeviceContext>& context, D3D11_VIEWPORT& screenViewport, const UINT width, const UINT height);
	static void CreateVertexShaderAndInputLayout(ComPtr<ID3D11Device>& device, const wstring& fileName, const vector<D3D11_INPUT_ELEMENT_DESC>& inputElements, ComPtr<ID3D11VertexShader>& vertexShader, ComPtr<ID3D11InputLayout>& inputLayout);
	static void CreatePixelShader(ComPtr<ID3D11Device>& device, const wstring& fileName, ComPtr<ID3D11PixelShader>& pixelShader);
	static void CreateTexture(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const string& filename, const bool useSRGB,
		ComPtr<ID3D11Texture2D>& tex, ComPtr<ID3D11ShaderResourceView>& srv, const UINT mipLevel);
	static void CreateDDSTexture(ComPtr<ID3D11Device>& device, const wchar_t* filename, ComPtr<ID3D11ShaderResourceView>& srv, bool isCubemap);
	static void ReadTexture(const string& filename, vector<uint8_t>& texBuffer, int& width, int& height);
};