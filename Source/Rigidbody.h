#pragma once

#include "Component.h"

class Rigidbody : public Component
{
public:
	Rigidbody(GameObject* pGameObject);
	~Rigidbody();

	// Component を介して継承されました
	virtual void Update() override;
	virtual void Draw() override {}

	D3DXVECTOR3 GetVelocity() { return m_velocity; }
	float GetMass() { return m_mass; }
	float GetBounciness() { return m_bounciness; }

	void SetVelocity(D3DXVECTOR3 velocity);
	void SetMass(float mass);

	void AddForce(D3DXVECTOR3 force);
private:
	D3DXVECTOR3 m_velocity;
	float m_mass; //質量
	D3DXVECTOR3 m_force;
	float m_friction;
	float m_bounciness;
};