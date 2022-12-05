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
	D3DXVECTOR3 GetWorldPosition();
	D3DXVECTOR3 GetPrevWorldPosition();
	D3DXQUATERNION GetRotation() { return m_quaternion; }
	D3DXVECTOR3 GetScale() { return m_scale; }
	D3DXVECTOR3 GetVelocity();
	D3DXVECTOR3 GetXAxis();
	D3DXVECTOR3 GetYAxis();
	D3DXVECTOR3 GetZAxis();

	void SetPosition(D3DXVECTOR3 position);
	void SetRotation(D3DXQUATERNION rotation);
	void AddQuaternion(D3DXQUATERNION quat);
	void SetScale(D3DXVECTOR3 scale);
	void SetCollisionScale(D3DXVECTOR3 scale);
	void SetToPrev();	

	D3DXVECTOR3 GetForward();
	D3DXVECTOR3 GetRight();
	D3DXMATRIX GetWorldMatrix();
	D3DXMATRIX GetCollisionScaleWorldMatrix();
	D3DXMATRIX GetPrevWorldMatrix();
	D3DXMATRIX GetWorldMatrixInvView();

	GameObject* GetParent();
	void SetParent(GameObject* parent);

private:
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_prevPosition;
	D3DXQUATERNION m_quaternion;
	D3DXQUATERNION m_prevQuaternion;
	D3DXVECTOR3 m_scale;
	D3DXVECTOR3 m_prevScale;
	D3DXVECTOR3 m_collisionScale;

	D3DXMATRIX m_worldMatrix;

	D3DXMATRIX m_collisionWorldMatrix;
	bool m_collisionChanged;

	GameObject* m_parent;

	bool m_changed;
	bool m_prevChanged;
};