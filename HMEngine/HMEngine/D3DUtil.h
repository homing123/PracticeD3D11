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

enum E_ShaderKind
{
	VS,
	HS,
	DS,
	GS,
	PS,
};

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
		// 주소 출력
		/*cout << "Source address: " << &cBufferData << endl;
		cout << "Destination address: " << ms.pData << endl;*/

		// 바이트 단위로 내용 출력
		//const unsigned char* bytes = reinterpret_cast<const unsigned char*>(&cBufferData);
		//cout << "Bytes to copy: ";
		//for (size_t i = 0; i < sizeof(T); ++i) {
		//	printf("%02X ", bytes[i]);
		//}
		//cout << endl;

		UINT size = sizeof(T);
		UINT size2 = sizeof(cBufferData);
		memcpy(ms.pData, &cBufferData, sizeof(cBufferData));
		context->Unmap(cBuffer.Get(), NULL);
		if (FAILED(hr))
		{
			cout << "UpdateCBuffer unmap failed" << endl;
		}
	}
	static void CreateVertexBuffer(ComPtr<ID3D11Device>& device, const vector<Vertex>& hBuffer, ComPtr<ID3D11Buffer>& dBuffer);
	static void CreateBillboardVertexIndexBuffer(ComPtr<ID3D11Device>& device, ComPtr<ID3D11Buffer>& vertexBuffer, ComPtr<ID3D11Buffer>& indexBuffer);
	static void CreateIndexBuffer(ComPtr<ID3D11Device>& device, const vector<uint32_t>& hBuffer, ComPtr<ID3D11Buffer>& dBuffer);
	static bool CreateDepthBuffer(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DepthStencilView>& depthStencilView, const UINT width, const UINT height, const int qualitylevels);
	static void SetViewport(ComPtr<ID3D11DeviceContext>& context, D3D11_VIEWPORT& screenViewport, const UINT width, const UINT height);
	static void CreateVertexShaderAndInputLayout(ComPtr<ID3D11Device>& device, const wstring& fileName, const vector<D3D11_INPUT_ELEMENT_DESC>& inputElements, ComPtr<ID3D11VertexShader>& vs, ComPtr<ID3D11InputLayout>& inputLayout);
	static void CreateHullShader(ComPtr<ID3D11Device>& device, const wstring& fileName, ComPtr<ID3D11HullShader>& hs);
	static void CreateDomainShader(ComPtr<ID3D11Device>& device, const wstring& fileName, ComPtr<ID3D11DomainShader>& ds);
	static void CreateGeometryShader(ComPtr<ID3D11Device>& device, const wstring& fileName, ComPtr<ID3D11GeometryShader>& gs);
	static void CreatePixelShader(ComPtr<ID3D11Device>& device, const wstring& fileName, ComPtr<ID3D11PixelShader>& ps);
	static void CreateTexture(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const string& filename, const bool useSRGB,
		ComPtr<ID3D11Texture2D>& tex, ComPtr<ID3D11ShaderResourceView>& srv, const UINT mipLevel);
	static void CreateDDSTexture(ComPtr<ID3D11Device>& device, const wchar_t* filename, ComPtr<ID3D11ShaderResourceView>& srv, bool isCubemap);
	static void ReadTexture(const string& filename, vector<uint8_t>& texBuffer, int& width, int& height);

	static void CreateMousePickingResources(ComPtr<ID3D11Device>& device, ComPtr<ID3D11Texture2D>& rtvTex, ComPtr<ID3D11RenderTargetView>& rtv, ComPtr<ID3D11Texture2D>& stagingTex, const UINT width, const UINT height);
	static bool CreateRenderTargetShaderResourceView(ComPtr<ID3D11Device>& device, ComPtr<IDXGISwapChain>& swapChain, ComPtr<ID3D11RenderTargetView>& renderTargetView, ComPtr<ID3D11ShaderResourceView>& shaderResourceView);
};