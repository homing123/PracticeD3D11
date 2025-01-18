#pragma once

#include "D3DUtil.h"

#include "Vertex.h"

using namespace std;

struct Mesh
{
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;

	ComPtr<ID3D11Texture2D> albedoTex;
	ComPtr<ID3D11Texture2D> emissiveTex;
	ComPtr<ID3D11Texture2D> normalTex;
	ComPtr<ID3D11Texture2D> heightTex; //height 랑 ao도 합칠수 있지 않을까?
	ComPtr<ID3D11Texture2D> aoTex;
	ComPtr<ID3D11Texture2D> metalicRoughnessTex;

	ComPtr<ID3D11ShaderResourceView> albedoSRV;
	ComPtr<ID3D11ShaderResourceView> emissiveSRV;
	ComPtr<ID3D11ShaderResourceView> normalSRV;
	ComPtr<ID3D11ShaderResourceView> heightSRV;
	ComPtr<ID3D11ShaderResourceView> aoSRV;
	ComPtr<ID3D11ShaderResourceView> metalicRoughnessSRV;
	
	UINT indexCount = 0;
	UINT vertexCount = 0;
	UINT stride = 0;
	UINT offset = 0;

};