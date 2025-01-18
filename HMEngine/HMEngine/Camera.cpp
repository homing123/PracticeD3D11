#include "Camera.h"

const Matrix Camera::GetProjectionMat() const
{
	//return DirectX::XMMatrixPerspectiveFovLH(XMConvertToRadians(m_FOV), m_AspectRatio, m_Near, m_Far);
	float invFarMinusNear = 1 / (m_Far - m_Near);
	float inv_tan = 1 / tanf(XMConvertToRadians(m_FOV * 0.5f));

	return Matrix(
					inv_tan / m_AspectRatio, 0, 0, 0,
					0, inv_tan, 0, 0,
					0, 0, m_Far * invFarMinusNear, 1,
					0, 0, -m_Near * m_Far * invFarMinusNear, 0);
}
const Matrix Camera::GetViewMat() const
{
	Matrix MT_Inv = Matrix::CreateTranslation(m_Transform->GetPosition() * -1);
	Matrix MR_Inv = Matrix::CreateFromQuaternion(m_Transform->GetRotation()).Transpose();
	return MT_Inv * MR_Inv;
}

Transform* Camera::GetPTransform()const
{
	return m_Transform.get();
}
void Camera::ChangeScreenSize(const int width, const int height)
{
	m_ScreenWidth = width;
	m_ScreenHeight = height;
	m_AspectRatio = (float)width / height;
}
