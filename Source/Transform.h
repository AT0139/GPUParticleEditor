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

	Vector3 GetPosition() { return m_position; }
	Vector3 GetPrevPosition() { return m_prevPosition; }
	Vector3 GetWorldPosition();
	Vector3 GetPrevWorldPosition();
	Quaternion GetRotation() { return m_quaternion; }
	Vector3 GetScale() { return m_scale; }
	Vector3 GetVelocity();
	Vector3 GetXAxis();
	Vector3 GetYAxis();
	Vector3 GetZAxis();

	void SetPosition(Vector3 position);
	void SetRotation(Quaternion rotation);
	void AddQuaternion(Quaternion quat);
	void SetScale(Vector3 scale);
	void SetCollisionScale(Vector3 scale);
	void SetToPrev();	

	Vector3 GetForward();
	Vector3 GetRight();
	Matrix GetWorldMatrix();
	Matrix GetCollisionScaleWorldMatrix();
	Matrix GetPrevWorldMatrix();
	Matrix GetWorldMatrixInvView();

	GameObject* GetParent();
	void SetParent(GameObject* parent);

private:
	Vector3 m_position;
	Vector3 m_prevPosition;
	Quaternion m_quaternion;
	Quaternion m_prevQuaternion;
	Vector3 m_scale;
	Vector3 m_prevScale;
	Vector3 m_collisionScale;
	Vector3 m_pivot;

	Matrix m_worldMatrix;

	Matrix m_collisionWorldMatrix;
	bool m_collisionChanged;

	GameObject* m_parent;

	bool m_changed;
	bool m_prevChanged;
};