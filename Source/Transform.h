#pragma once

#include "Component.h"
class Transform : public Component
{
public:
	Transform(GameObject* pGameObject);
	~Transform() {}

	// Component を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

	D3DXVECTOR3 GetPosition() { return m_position; }
	D3DXVECTOR3 GetPrevPosition() { return m_prevPosition; }
	D3DXVECTOR3 GetRotation() { return m_rotation; }
	D3DXVECTOR3 GetScale() { return m_scale; }
	D3DXVECTOR3 GetVelocity() { return m_position - m_prevPosition; }

	void SetPosition(D3DXVECTOR3 position) { m_position = position; }
	void SetRotation(D3DXVECTOR3 rotation) { m_rotation = rotation; }
	void SetScale(D3DXVECTOR3 scale) { m_scale = scale; }

	D3DXVECTOR3 GetForward();
	D3DXVECTOR3 GetRight();
	D3DXMATRIX GetWorldMatrix();
	D3DXMATRIX GetPrevWorldMatrix();
	D3DXMATRIX GetWorldMatrixInvView();
private:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_prevPosition;
	D3DXVECTOR3 m_rotation;
	D3DXVECTOR3 m_prevRotation;
	D3DXVECTOR3 m_scale;
	D3DXVECTOR3 m_prevScale;
};