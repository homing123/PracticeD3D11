#pragma once

#include <memory>
#include "Transform.h"

using namespace std;
class Camera
{
public:
	Camera() 
	{
		m_Transform = make_shared<Transform>();
	}

	const Matrix GetProjectionMat() const;
	const Matrix GetViewMat() const;
	Transform* GetPTransform() const;
	void ChangeScreenSize(const int width, const int height);
private:
	float m_FOV = 90;
	float m_Far = 100;
	float m_Near = 0.1f;
	float m_AspectRatio;
	uint32_t m_ScreenWidth;
	uint32_t m_ScreenHeight;
	shared_ptr<Transform> m_Transform;

};