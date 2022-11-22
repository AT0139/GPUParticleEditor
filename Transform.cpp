#include "stdafx.h"
#include "Transform.h"

Transform::Transform(const std::shared_ptr<GameObject> pGameObject)
	: Component(pGameObject)
{}

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
