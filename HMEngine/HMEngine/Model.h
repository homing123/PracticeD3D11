#pragma once

#include "D3DUtil.h"
#include "Mesh.h"
#include "MeshData.h"
#include "ConstantBuffer.h"

class Model
{
public:
	Model(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const vector<MeshData>& meshes);
	void Init(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const vector<MeshData>& meshes);

	bool m_UseAlbedTex;
	bool m_UseEmissiveTex;
	bool m_UseNormalTex;
	bool m_UseHeightTex;
	bool m_UseAOTex;

public:
	vector<shared_ptr<Mesh>> m_meshes;
};