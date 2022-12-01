#include "stdafx.h"
#include "Transform.h"
#include "Manager.h"
#include "Camera.h"

Transform::Transform(GameObject* pGameObject)
	: Component(pGameObject)
	, m_position(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, m_rotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, m_scale(D3DXVECTOR3(1.0f, 1.0f, 1.0f))
	, m_prevChangeed(false)
{}

void Transform::SetWorldPosition(D3DXVECTOR3 position)
{
	SetPosition(position);
}

void Transform::SetToPrev()
{
	if (m_prevScale != m_scale)
	{
		m_prevChangeed = true;
		m_prevScale = m_scale;
	}
	if (m_prevPosition != m_position)
	{
		m_prevChangeed = true;
		m_prevPosition = m_position;
	}
	if (m_prevRotation != m_rotation)
	{
		m_prevChangeed = true;
		m_prevRotation = m_rotation;
	}
}

D3DXVECTOR3 Transform::GetForward()
{
	D3DXMATRIX rot;
	D3DXMatrixRotationYawPitchRoll(&rot, m_rotation.y, m_rotation.x, m_rotation.z);

	D3DXVECTOR3 forward;
	forward.x = rot._31;
	forward.y = rot._32;
	forward.z = rot._33;

	return forward;
}

D3DXVECTOR3 Transform::GetRight()
{
	D3DXMATRIX rot;
	D3DXMatrixRotationYawPitchRoll(&rot, m_rotation.y, m_rotation.x, m_rotation.z);

	D3DXVECTOR3 right;
	right.x = rot._11;
	right.y = rot._12;
	right.z = rot._13;

	return right;
}

D3DXMATRIX Transform::GetWorldMatrix()
{
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_rotation.y, m_rotation.x, m_rotation.z);
	D3DXMatrixTranslation(&trans, m_position.x, m_position.y, m_position.z);

	return scale * rot * trans;
}

D3DXMATRIX Transform::GetPrevWorldMatrix()
{
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_prevScale.x, m_prevScale.y, m_prevScale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_prevRotation.y, m_prevRotation.x, m_prevRotation.z);
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

void Transform::Update()
{
	SetToPrev();
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
