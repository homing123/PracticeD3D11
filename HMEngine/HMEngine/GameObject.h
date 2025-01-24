#pragma once

#include <string>
#include "D3DUtil.h"
#include "ImGuiUtil.h"
#include "Model.h"
#include "Transform.h"
#include "Material.h"
#include "GraphicsPSO.h"

using namespace std;
using Microsoft::WRL::ComPtr;
class GameObject
{
public:
	GameObject(ComPtr<ID3D11Device>& device, const string& name, Model* pModel, GraphicsPSO* pPSO);
	Transform* GetPTransform() const;
	void Render(ComPtr<ID3D11DeviceContext>& context);
	void RenderUseCustomPSO(ComPtr<ID3D11DeviceContext>& context);
	void DrawGui(ComPtr<ID3D11DeviceContext>& context);

public:
	string m_Name;
	size_t m_NameKey = 0;
	bool m_Active = true;
	GraphicsPSO* m_PSO = nullptr;
	Model* m_Model = nullptr;

private:
	GameObject() = default;

private:
	static vector<shared_ptr<GameObject>> Objs;
	shared_ptr<Transform> m_Transform;


};