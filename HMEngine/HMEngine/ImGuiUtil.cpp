
#include "ImGuiUtil.h"

const void ImGuiUtil::DrawTransform(Transform* pTF)
{
	if (ImGui::TreeNode("Transform")) 
	{
		Vector3 pos = pTF->GetPosition();
		Vector3 rot = pTF->GetRotation().ToEuler();
		Vector3 scale = pTF->GetScale();

		ImGui::DragFloat3("Position", &pos.x, 0.05f);
		ImGui::DragFloat3("Rotation", &rot.x, 0.01f);
		ImGui::DragFloat3("Scale", &scale.x, 0.02f);

		pTF->SetPosition(pos);
		pTF->SetEuler(rot);
		pTF->SetScale(scale);
		ImGui::TreePop();
	}
}
const void ImGuiUtil::DrawMaterial(MaterialCBuffer* pMat, const E_MatKind kind)
{
	if (ImGui::TreeNode("Material"))
	{
		switch (kind)
		{
		case E_MatKind::BlinnPhong:
		{
			BlinnPhongCBuffer* pBlinnPhong = static_cast<BlinnPhongCBuffer*>(pMat);
			ImGui::SliderFloat3("Ambient", &pBlinnPhong->ambient.x, 0, 1);
			ImGui::SliderFloat3("Diffuse", &pBlinnPhong->diffuse.x, 0, 1);
			ImGui::SliderFloat3("Specular", &pBlinnPhong->specular.x, 0, 1);
			ImGui::SliderFloat("Shininess", &pBlinnPhong->shininess, 0, 50);
		}
		break;
		case E_MatKind::PBR:

			//D3DUtil::CreateCBuffer(device, *static_cast<PBRCBuffer*>(m_MaterialCBufferCPU), m_MaterialCBufferGPU);
			break;
		case E_MatKind::Skybox:
			//D3DUtil::CreateCBuffer(device, *static_cast<SkyboxCBuffer*>(m_MaterialCBufferCPU), m_MaterialCBufferGPU);
			break;
		case E_MatKind::MousePicking:
			//D3DUtil::CreateCBuffer(device, *static_cast<MousePickingCBuffer*>(m_MaterialCBufferCPU), m_MaterialCBufferGPU);
			break;
		}
		ImGui::TreePop();
	}
}
