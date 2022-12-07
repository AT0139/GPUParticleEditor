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

	Vector3 GetVelocity() { return m_velocity; }
	float GetMass() { return m_mass; }
	float GetBounciness() { return m_bounciness; }

	void SetVelocity(Vector3 velocity);
	void SetVelocityZero();
	void SetGravity(Vector3 gravity);
	void SetGravityZero();
	void SetMass(float mass);

	void AddForce(Vector3 force);
private:
	Vector3 m_velocity;
	Vector3 m_gravity;
	float m_mass; //質量
	Vector3 m_force;
	float m_friction;
	float m_bounciness;
};