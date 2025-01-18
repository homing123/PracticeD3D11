#pragma once

#include <string>
#include "D3DUtil.h"
#include "Model.h"
#include "Transform.h"
#include "Material.h"
#include "GraphicsPSO.h"

using namespace std;
using Microsoft::WRL::ComPtr;
class GameObject
{
public:
	GameObject() {};
	GameObject(ComPtr<ID3D11Device>& device, const string& name, Model* pModel, GraphicsPSO* pPSO);
	size_t m_NameKey = 0;

	Transform* GetPTransform() const;
	void Render(ComPtr<ID3D11DeviceContext>& context);

	bool m_Active = true;
	GraphicsPSO* m_PSO = nullptr;
	Model* m_Model = nullptr;

private:
	static vector<shared_ptr<GameObject>> Objs;

	shared_ptr<Transform> m_Transform;


};