#include "Transform.h"
Transform::Transform(ComPtr<ID3D11Device>& device)
{
	D3DUtil::CreateCBuffer(device, m_TransformCBufferCPU, m_TransformCBufferGPU);
}
const Vector3 Transform::GetPosition() const { return m_Position; }
const Quaternion Transform::GetRotation() const { return m_Rotation; }
const Vector3 Transform::GetScale() const { return m_Scale; }
const Matrix Transform::GetWorldMat() const { return m_WorldMat; }
const Vector3 Transform::GetForward() const { return m_Forward; }
const Vector3 Transform::GetUp() const { return m_Up; }
const Vector3 Transform::GetRight() const { return m_Right; }

void Transform::SetPosition(const Vector3& _position)
{
	m_Position = _position;
	Update();
}

void Transform::Translate(const Vector3& move)
{
	m_Position += move;
	Update();
}

void Transform::SetRotation(const Quaternion& _rotation)
{
	m_Rotation = _rotation;
	Update();
}
void Transform::SetEuler(const Vector3& euler_radian)
{
	m_Rotation = m_Rotation.CreateFromYawPitchRoll(euler_radian.y, euler_radian.x, euler_radian.z);
	Update();
}
void Transform::RotationX(const float degree)
{
	Vector3 euler = m_Rotation.ToEuler();
	m_Rotation = m_Rotation.CreateFromYawPitchRoll(euler.y, XMConvertToRadians(degree), euler.z);
	Update();
}
void Transform::RotationY(const float degree)
{
	Vector3 euler = m_Rotation.ToEuler();
	m_Rotation = m_Rotation.CreateFromYawPitchRoll(XMConvertToRadians(degree), euler.x, euler.z);	Update();
	Update();
}
void Transform::RotationZ(const float degree)
{
	Vector3 euler = m_Rotation.ToEuler();
	m_Rotation = m_Rotation.CreateFromYawPitchRoll(euler.y, euler.x, XMConvertToRadians(degree));
	Update();
}
void Transform::SetScale(const Vector3& _scale)
{
	m_Scale = _scale;
	Update();
}


void Transform::Update()
{
	m_WorldMat = Matrix::CreateScale(m_Scale) * Matrix::CreateFromQuaternion(m_Rotation) * Matrix::CreateTranslation(m_Position) ;
	m_Forward = -m_WorldMat.Forward();
	m_Right = m_WorldMat.Right();
	m_Up = m_WorldMat.Up();

	m_isChanged = true;
}

void Transform::SetTransformCBuffer(ComPtr< ID3D11DeviceContext>& context)
{
	if (m_isChanged)
	{
		m_TransformCBufferCPU.world = m_WorldMat.Transpose(); //d3d11 = row행렬, hlsl = col행렬 => 넣을때 transpose해줘야함
		m_TransformCBufferCPU.worldIT = m_WorldMat.Invert(); //invert, transpose, transpose 라서 생략됨
		D3DUtil::UpdateCBuffer(context, m_TransformCBufferCPU, m_TransformCBufferGPU);
		m_isChanged = false;
	}

	context->VSSetConstantBuffers(TRANSFORM_CBUFFER_SLOT, 1, m_TransformCBufferGPU.GetAddressOf());
}


void Transform::DrawGui()
{
	ImGui::Text("Transform");

	Vector3 rot = m_Rotation.ToEuler();

	ImGui::DragFloat3("Position", &m_Position.x, 0.05f);
	ImGui::DragFloat3("Rotation", &rot.x, 0.01f);
	ImGui::DragFloat3("Scale", &m_Scale.x, 0.02f);

	SetEuler(rot);
}