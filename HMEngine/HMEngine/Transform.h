#pragma once

#include <directxtk/SimpleMath.h>
#include "ConstantBuffer.h"
#include "D3DUtil.h"
#include "ImGuiUtil.h"

using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Quaternion;
using namespace DirectX;

class Transform
{

public:

	Transform(ComPtr<ID3D11Device>& device);
	const Vector3 GetPosition() const;
	const Quaternion GetRotation() const;
	const Vector3 GetScale() const;
	const Matrix GetWorldMat() const;
	const Vector3 GetForward() const;
	const Vector3 GetUp() const;
	const Vector3 GetRight() const;

	void SetPosition(const Vector3& _position);
	void Translate(const Vector3& move);

	void SetRotation(const Quaternion& _rotation);
	void SetEuler(const Vector3& euler_radian);
	void RotationX(const float degree);
	void RotationY(const float degree);
	void RotationZ(const float degree);

	void SetScale(const Vector3& _scale);

	void SetTransformCBuffer(ComPtr<ID3D11DeviceContext>& context);
	void DrawGui();

private:
	Transform() = default;

private:
	Vector3 m_Position;
	Quaternion m_Rotation;
	Vector3 m_Scale = Vector3::One;

	Vector3 m_Forward = Vector3::UnitZ;
	Vector3 m_Up = Vector3::UnitY;
	Vector3 m_Right = Vector3::UnitX;

	Matrix m_WorldMat = Matrix::Identity;
	TransformCBuffer m_TransformCBufferCPU;
	ComPtr<ID3D11Buffer> m_TransformCBufferGPU;
	bool m_isChanged = false;
	void Update();
};