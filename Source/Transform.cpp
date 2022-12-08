#include "stdafx.h"
#include "Transform.h"
#include "Manager.h"
#include "Camera.h"

Transform::Transform(GameObject* pGameObject)
	: Component(pGameObject)
	, m_position(Vector3(0.0f, 0.0f, 0.0f))
	, m_quaternion(XMQuaternionIdentity())
	, m_scale(Vector3(1.0f, 1.0f, 1.0f))
	, m_changed(true)
	, m_prevChanged(true)
	, m_parent(nullptr)
	, m_pivot(m_position)
{}

void Transform::SetPosition(Vector3 position)
{
	m_position = position;
	m_pivot = position;
	m_changed = true;
}

void Transform::SetRotation(Quaternion quat)
{
	m_quaternion = quat;
	m_changed = true;
}

void Transform::AddQuaternion(Quaternion quat)
{
	if (!XMVector3Equal(Vector3(0.0f, 0.0f, 0.0f), quat))
	{
		m_quaternion = XMQuaternionMultiply(m_quaternion, quat);
		m_changed = true;
	}
}

void Transform::SetScale(Vector3 scale)
{
	m_scale = scale;
	m_changed = true;
}

void Transform::SetToPrev()
{
	if (m_prevScale != m_scale)
	{
		m_prevChanged = true;
		m_prevScale = m_scale;
	}
	if (m_prevPosition != m_position)
	{
		m_prevChanged = true;
		m_prevPosition = m_position;
	}
	if (m_prevQuaternion != m_quaternion)
	{
		m_prevChanged = true;
		m_prevQuaternion = m_quaternion;
	}
}

Vector3 Transform::GetForward()
{
	Matrix rot;
	XMFLOAT4X4 tmp;
	rot = XMMatrixRotationQuaternion(m_quaternion);
	XMStoreFloat4x4(&tmp, rot);
	Vector3 forward;
	forward.x = tmp._31;
	forward.y = tmp._32;
	forward.z = tmp._33;

	return forward;
}

Vector3 Transform::GetRight()
{
	Matrix rot;
	XMFLOAT4X4 tmp;
	rot = XMMatrixRotationQuaternion(m_quaternion);
	XMStoreFloat4x4(&tmp, rot);
	Vector3 right;
	right.x = tmp._11;
	right.y = tmp._12;
	right.z = tmp._13;

	return right;
}

Matrix Transform::GetWorldMatrix()
{
	auto parent = GetParent();
	if (m_changed || parent)
	{
		Matrix scale, rot, trans;
		scale = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
		rot = XMMatrixRotationQuaternion(m_quaternion);
		trans = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
		m_worldMatrix = scale * rot * trans;
		m_changed = false;

		if (parent)
		{
			auto parentWorld = parent->GetComponent<Transform>()->GetWorldMatrix();
			parentWorld.ScaleIdentity();
			m_worldMatrix = m_worldMatrix * parentWorld;
		}
	}

	return m_worldMatrix;
}

Matrix Transform::GetWorldMatrix(Vector3 scale)
{
	auto parent = GetParent();
	Matrix world;

	Matrix sc, rot, trans;
	sc = XMMatrixScaling(scale.x, scale.y, scale.z);
	rot = XMMatrixRotationQuaternion(m_quaternion);
	trans = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	world = sc * rot * trans;
	if (parent)
	{
		auto parentWorld = parent->GetComponent<Transform>()->GetWorldMatrix();
		parentWorld.ScaleIdentity();
		world = world * parentWorld;

		return world;
	}

	return world;
}

Matrix Transform::GetPrevWorldMatrix()
{
	auto parent = GetParent();

	Matrix scale, rot, trans;
	scale = XMMatrixScaling(m_prevScale.x, m_prevScale.y, m_prevScale.z);
	rot = XMMatrixRotationQuaternion(m_prevQuaternion);
	trans = XMMatrixTranslation(m_prevPosition.x, m_prevPosition.y, m_prevPosition.z);

	auto world = scale * rot * trans;
	if (parent)
	{
		auto parentWorld = parent->GetComponent<Transform>()->GetWorldMatrix();
		parentWorld.ScaleIdentity();
		m_worldMatrix = world * parentWorld;
	}

	return world;
}

Matrix Transform::GetWorldMatrixInvView()
{
	//ビューの逆行列
	Scene* scene = Manager::GetInstance().GetScene();
	MainGame::Camera* camera = scene->GetGameObject<MainGame::Camera>(scene->CAMERA);
	Matrix view = camera->GetViewMatrix();
	Matrix invView = XMMatrixInverse(NULL, view);

	XMFLOAT4X4 tmp;
	XMStoreFloat4x4(&tmp, invView);
	tmp._41 = 0.0f;
	tmp._42 = 0.0f;
	tmp._43 = 0.0f;
	invView = XMLoadFloat4x4(&tmp);

	//ワールドマトリクス設定
	Matrix world, scale, trans;
	scale = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	trans = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	world = scale * invView * trans;
	return world;
}

GameObject* Transform::GetParent()
{
	return m_parent;
}

void Transform::SetParent(GameObject* parent)
{
	if (m_parent == parent)
		return;

	if (parent)
	{
		ResetParent();
		m_parent = parent;
		Matrix parentWorld = parent->GetComponent<Transform>()->GetWorldMatrix();
		parentWorld.ScaleIdentity();
		auto posSpan = GetPosition() - parentWorld.TransInMatrix();
		auto quatSpan = parentWorld.QuaternionInMatrix();
		quatSpan = XMQuaternionInverse(quatSpan);
		Matrix parentQuatMatrix(quatSpan);
		posSpan *= parentQuatMatrix;

		Matrix mat = GetWorldMatrix() * parentWorld;
		Vector3 scale, pos;
		Quaternion quat;
		mat.Division(scale, quat, pos);
		SetScale(scale);
		SetRotation(quat);
		SetPosition(posSpan);
		SetToPrev();
	}
}

void Transform::ResetParent()
{
	auto parent = GetParent();
	if (parent)
	{
		auto pos = GetWorldPosition();
		SetPosition(pos);
		SetToPrev();
	}
	m_parent = nullptr;
}

void Transform::Update()
{
	SetToPrev();
}

void Transform::Draw()
{
}

Vector3 Transform::GetWorldPosition()
{
	Vector3 pos;
	XMFLOAT4X4 tmp;

	auto world = GetWorldMatrix();
	XMStoreFloat4x4(&tmp, world);
	pos.x = tmp._41;
	pos.y = tmp._42;
	pos.z = tmp._43;
	return pos;
}

Vector3 Transform::GetPrevWorldPosition()
{
	Vector3 pos;
	XMFLOAT4X4 tmp;
	auto world = GetPrevWorldMatrix();
	XMStoreFloat4x4(&tmp, world);
	pos.x = tmp._41;
	pos.y = tmp._42;
	pos.z = tmp._43;
	return pos;
}

Vector3 Transform::GetVelocity()
{
	Vector3 velo = m_position - m_prevPosition;
	velo /= FPS;
	return velo;
}

Vector3 Transform::GetXAxis()
{
	Matrix world = GetWorldMatrix();
	Vector3 v;
	XMFLOAT4X4 tmp;
	XMStoreFloat4x4(&tmp, world);

	v.x = tmp._11;
	v.y = tmp._12;
	v.z = tmp._13;

	//todo : サイズ考慮
	return v;
}

Vector3 Transform::GetYAxis()
{
	Matrix world = GetWorldMatrix();

	Vector3 v;
	XMFLOAT4X4 tmp;
	XMStoreFloat4x4(&tmp, world);

	v.x = tmp._21;
	v.y = tmp._22;
	v.z = tmp._23;

	//todo : サイズ考慮
	return v;
}

Vector3 Transform::GetZAxis()
{
	Matrix world = GetWorldMatrix();

	Vector3 v;
	XMFLOAT4X4 tmp;
	XMStoreFloat4x4(&tmp, world);

	v.x = tmp._31;
	v.y = tmp._32;
	v.z = tmp._33;

	//todo : サイズ考慮
	return v;
}