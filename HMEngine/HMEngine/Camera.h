#pragma once

#include <memory>
#include "Transform.h"
#include "ImGuiUtil.h"

using namespace std;
class Camera
{
public:
	Camera(ComPtr<ID3D11Device>& device) 
	{
		m_Transform = make_shared<Transform>(device);
	}

	const Matrix GetProjectionMat() const;
	const Matrix GetViewMat() const;
	Transform* GetPTransform() const;
	void ChangeScreenSize(const int width, const int height);
	void DrawGui(ComPtr<ID3D11DeviceContext>& context);
private:
	float m_FOV = 90;
	float m_Far = 100;
	float m_Near = 0.1f;
	float m_AspectRatio;
	uint32_t m_ScreenWidth;
	uint32_t m_ScreenHeight;
	shared_ptr<Transform> m_Transform;

};