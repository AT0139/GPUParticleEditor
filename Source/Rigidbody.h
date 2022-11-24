#pragma once

#include "Component.h"

class Rigidbody : public Component
{
public:
	Rigidbody(GameObject* pGameObject);
	~Rigidbody();

	// Component を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

	void SetVelocity(D3DXVECTOR3 velocity);
	void SetMass(float mass);

	void AddForce(D3DXVECTOR3 force);
private:
	D3DXVECTOR3 m_velocity;
	float m_mass; //質量

};