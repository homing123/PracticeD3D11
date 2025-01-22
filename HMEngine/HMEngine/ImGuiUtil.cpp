
#include "ImGuiUtil.h"

const void ImGuiUtil::DrawTransform(Transform* pTF)
{
	Vector3 pos = pTF->GetPosition();
	float arr_pos[3] = { pos.x, pos.y, pos.z };
	Vector3 rot = pTF->GetRotation().ToEuler();
	float arr_rot[3] = { rot.x, rot.y, rot.z };
	Vector3 scale = pTF->GetScale();
	float arr_scale[3] = { scale.x, scale.y, scale.z };

	ImGui::DragFloat3("Position", arr_pos, 0.05f);
	ImGui::DragFloat3("Rotation", arr_rot, 0.01f);
	ImGui::DragFloat3("Scale", arr_scale, 0.02f);

	pos.x = arr_pos[0];
	pos.y = arr_pos[1];
	pos.z = arr_pos[2];
	rot.x = arr_rot[0];
	rot.y = arr_rot[1];
	rot.z = arr_rot[2];
	scale.x = arr_scale[0];
	scale.y = arr_scale[1];
	scale.z = arr_scale[2];
	pTF->SetPosition(pos);
	pTF->SetEuler(rot);
	pTF->SetScale(scale);
}