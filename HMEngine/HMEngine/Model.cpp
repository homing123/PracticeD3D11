#include "Model.h"

Model::Model(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const vector<MeshData>& meshes)
{
	Init(device, context, meshes);
}
void Model::Init(ComPtr<ID3D11Device>& device, ComPtr<ID3D11DeviceContext>& context, const vector<MeshData>& meshes)
{
	for (const auto& meshData : meshes)
	{
		auto mesh = make_shared<Mesh>();
		mesh->indexCount = meshData.indices.size();
		mesh->vertexCount = meshData.vertices.size();
		mesh->stride = UINT(sizeof(Vertex));
		D3DUtil::CreateVertexBuffer(device, meshData.vertices, mesh->vertexBuffer);
		D3DUtil::CreateIndexBuffer(device, meshData.indices, mesh->indexBuffer);

		if (meshData.albedoTexFilename.empty() == false)
		{
			D3DUtil::CreateTexture(device, context, meshData.albedoTexFilename, true, mesh->albedoTex, mesh->albedoSRV, 0);
			m_UseAlbedTex = true;
		}
		if (meshData.emissiveTexFilename.empty() == false)
		{
			D3DUtil::CreateTexture(device, context, meshData.emissiveTexFilename, true, mesh->emissiveTex, mesh->emissiveSRV, 0);
			m_UseEmissiveTex = true;
		}
		if (meshData.normalTexFilename.empty() == false)
		{
			D3DUtil::CreateTexture(device, context, meshData.normalTexFilename, false, mesh->normalTex, mesh->normalSRV, 0);
			m_UseNormalTex = true;
		}
		if (meshData.heightTexFilename.empty() == false)
		{
			D3DUtil::CreateTexture(device, context, meshData.heightTexFilename, false, mesh->heightTex, mesh->heightSRV, 0);
			m_UseHeightTex = true;
		}
		if (meshData.aoTexFilename.empty() == false)
		{
			D3DUtil::CreateTexture(device, context, meshData.aoTexFilename, false, mesh->aoTex, mesh->aoSRV, 0);
			m_UseAOTex = true;
		}


		//합쳐서 사용
		/*if (meshData.metalicTexFilename.empty() == false)
		{
			D3DUtil::CreateTexture(device, context, meshData.metalicTexFilename, false, mesh->metalicRoughnessTex, mesh->albedoSRV, 0);
			m_MaterialCBufferCPU.useAlbedo = true;
		}
		if (meshData.roughnessTexFilename.empty() == false)
		{
			D3DUtil::CreateTexture(device, context, meshData.albedoTexFilename, false, mesh->albedoTex, mesh->albedoSRV, 0);
			m_MaterialCBufferCPU.useAlbedo = true;
		}*/

		m_meshes.push_back(mesh);
	}
}