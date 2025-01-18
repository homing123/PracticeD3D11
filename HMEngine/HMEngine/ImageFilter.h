//#pragma once
//
//#include "D3DUtil.h"
//#include "Mesh.h"
//#include "Material.h"
//
//using DirectX::SimpleMath::Vector4;
//
//struct SamplingPSCBuffer
//{
//	float dx;
//	float dy;
//	float threshold;
//	float strength;
//	Vector4 options;
//};
//
//class ImageFilter
//{
//public:
//	void Init(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, Mesh* pMesh, const wstring& psName, const int width, const int height);
//	void SetShaderResources(const vector<ComPtr<ID3D11ShaderResourceView>>& resources);
//	void SetRenderTargets(const vector<ComPtr<ID3D11RenderTargetView>>& renderTargets);
//	void UpdateCBuffer(ID3D11DeviceContext* pContext);
//	void Render(ID3D11DeviceContext* pContext);
//	ComPtr<ID3D11ShaderResourceView> m_ShaderResourceView;
//	ComPtr<ID3D11RenderTargetView> m_RenderTargetView;
//	SamplingPSCBuffer m_CBufferCPU;
//	static string MeshName;
//
//private:
//	static wstring VSName;
//	Mesh* m_Mesh;
//	ComPtr<ID3D11VertexShader> m_VS;
//	ComPtr<ID3D11PixelShader> m_PS;
//	ComPtr<ID3D11InputLayout> m_InputLayout;
//	ComPtr<ID3D11SamplerState> m_SamplerState;
//	ComPtr<ID3D11RasterizerState> m_RasterizerState;
//	ComPtr<ID3D11Buffer> m_CBufferGPU;
//	D3D11_VIEWPORT m_Viewport;
//
//	vector<ID3D11ShaderResourceView*> m_ShaderResourceViews;
//	vector<ID3D11RenderTargetView*> m_RenderTargets;
//
//
//};
