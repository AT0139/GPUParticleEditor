#include "stdafx.h"
#include "Transform.h"
#include "Manager.h"
#include "Camera.h"

Transform::Transform(GameObject* pGameObject)
	: Component(pGameObject)
	, m_position(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, m_quaternion(D3DXQUATERNION(0.0f, 0.0f, 0.0f,0.0f))
	, m_scale(D3DXVECTOR3(1.0f, 1.0f, 1.0f))
	, m_changed(true)
	, m_prevChanged(true)
	, m_collisionScale(m_scale)
	, m_collisionChanged(true)
	, m_parent(nullptr)
{}

void Transform::SetPosition(D3DXVECTOR3 position)
{
	m_position = position;
	m_changed = true;
	m_collisionChanged = true;
}

void Transform::SetRotation(D3DXQUATERNION quat)
{
	m_quaternion = quat;
	m_changed = true;
	m_collisionChanged = true;
}

void Transform::AddQuaternion(D3DXQUATERNION quat)
{
	D3DXQuaternionMultiply(&m_quaternion, &m_quaternion, &quat);
	m_changed = true;
	m_collisionChanged = true;
}

void Transform::SetScale(D3DXVECTOR3 scale)
{
	m_scale = scale;
	m_changed = true;
}

void Transform::SetCollisionScale(D3DXVECTOR3 scale)
{
	m_collisionScale = scale;
	m_collisionChanged = true;
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

D3DXVECTOR3 Transform::GetForward()
{
	D3DXMATRIX rot;
	D3DXMatrixRotationQuaternion(&rot, &m_quaternion);

	D3DXVECTOR3 forward;
	forward.x = rot._31;
	forward.y = rot._32;
	forward.z = rot._33;

	return forward;
}

D3DXVECTOR3 Transform::GetRight()
{
	D3DXMATRIX rot;
	D3DXMatrixRotationQuaternion(&rot, &m_quaternion);

	D3DXVECTOR3 right;
	right.x = rot._11;
	right.y = rot._12;
	right.z = rot._13;

	return right;
}

D3DXMATRIX Transform::GetWorldMatrix()
{
	auto parent = GetParent();
	if (m_changed || parent)
	{
		D3DXMATRIX world, scale, rot, trans;
		D3DXMatrixScaling(&scale, m_scale.x, m_scale.y, m_scale.z);
		D3DXMatrixRotationQuaternion(&rot, &m_quaternion);
		D3DXMatrixTranslation(&trans, m_position.x, m_position.y, m_position.z);
		m_worldMatrix = scale * rot * trans;

		m_changed = false;

		if (parent)
		{
			auto parentWorld = parent->GetComponent<Transform>()->GetWorldMatrix();
			
		}
	}

	


	return m_worldMatrix;
}

D3DXMATRIX Transform::GetCollisionScaleWorldMatrix()
{
	if (m_collisionChanged)
	{
		D3DXMATRIX world, scale, rot, trans;
		D3DXMatrixScaling(&scale, m_collisionScale.x, m_collisionScale.y, m_collisionScale.z);
		D3DXMatrixRotationQuaternion(&rot, &m_quaternion);
		D3DXMatrixTranslation(&trans, m_position.x, m_position.y, m_position.z);
		m_collisionWorldMatrix = scale * rot * trans;
	}
	m_collisionChanged = false;
	return m_collisionWorldMatrix;
}

D3DXMATRIX Transform::GetPrevWorldMatrix()
{
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_prevScale.x, m_prevScale.y, m_prevScale.z);
	D3DXMatrixRotationQuaternion(&rot, &m_prevQuaternion);
	D3DXMatrixTranslation(&trans, m_prevPosition.x, m_prevPosition.y, m_prevPosition.z);

	return scale * rot * trans;
}

D3DXMATRIX Transform::GetWorldMatrixInvView()
{
	//ビューの逆行列
	Scene* scene = Manager::GetInstance().GetScene();
	MainGame::Camera* camera = scene->GetGameObject<MainGame::Camera>(scene->CAMERA);
	D3DXMATRIX view = camera->GetViewMatrix();
	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, NULL, &view);
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;

	//ワールドマトリクス設定
	D3DXMATRIX world, scale, trans;
	D3DXMatrixScaling(&scale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixTranslation(&trans, m_position.x, m_position.y, m_position.z);
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
		m_parent = parent;
	}
}

void Transform::Update()
{
	SetToPrev();
	m_collisionChanged = false;
}

void Transform::Draw()
{
}

D3DXVECTOR3 Transform::GetWorldPosition()
{
	D3DXVECTOR3 pos;
	auto world = GetWorldMatrix();
	pos.x = world._41;
	pos.y = world._42;
	pos.z = world._43;
	return pos;
}

D3DXVECTOR3 Transform::GetPrevWorldPosition()
{
	D3DXVECTOR3 pos;
	auto world = GetPrevWorldMatrix();
	pos.x = world._41;
	pos.y = world._42;
	pos.z = world._43;
	return pos;
}

D3DXVECTOR3 Transform::GetVelocity()
{
	D3DXVECTOR3 velo = m_position - m_prevPosition;
	velo /= FPS;
	return velo;
}

D3DXVECTOR3 Transform::GetXAxis()
{
	D3DXMATRIX world = GetCollisionScaleWorldMatrix();

	D3DXVECTOR3 v;
	v.x = world._11;
	v.y = world._12;
	v.z = world._13;

	//todo : サイズ考慮
	return v;
}

D3DXVECTOR3 Transform::GetYAxis()
{
	D3DXMATRIX world = GetCollisionScaleWorldMatrix();

	D3DXVECTOR3 v;
	v.x = world._21;
	v.y = world._22;
	v.z = world._23;

	//todo : サイズ考慮
	return v;
}

D3DXVECTOR3 Transform::GetZAxis()
{
	D3DXMATRIX world = GetCollisionScaleWorldMatrix();

	D3DXVECTOR3 v;
	v.x = world._31;
	v.y = world._32;
	v.z = world._33;

	//todo : サイズ考慮
	return v;
}
