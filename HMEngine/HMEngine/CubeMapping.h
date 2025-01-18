//#pragma once
//
//#include <directxtk/DDSTextureLoader.h> // 큐브맵 읽을 때 필요
//#include <directxtk/WICTextureLoader.h>
//#include "D3DUtil.h"
//#include "Material.h"
//
//using Microsoft::WRL::ComPtr;
//using namespace std;
//using namespace DirectX;
//using namespace Microsoft::WRL;
//class CubeMapping
//{
//public:
//	void Init(ID3D11Device* device);
//	void UpdateVSCBuffer(ID3D11DeviceContext* pContext, const Matrix& MatView, const Matrix& MatProj);
//
//	const static size_t MeshKey;
//	ComPtr<ID3D11ShaderResourceView> m_DiffuseResourceView;
//	ComPtr<ID3D11ShaderResourceView> m_SpecularResourceView;
//	ComPtr<ID3D11VertexShader> m_VS;
//	ComPtr<ID3D11PixelShader> m_PS;
//	ComPtr<ID3D11Buffer> m_VSCBuffer;
//	ComPtr<ID3D11Buffer> m_PSCBuffer;
//	ComPtr<ID3D11InputLayout> m_InputLayout;
//};