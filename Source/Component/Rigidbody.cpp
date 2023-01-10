#include "Rigidbody.h"
#include "Transform.h"
#include "GameObject.h"
#include "Scene.h"
#include "SceneManager.h"
#include "MeshField.h"

static const float GRAVITY = 1.0f;

Rigidbody::Rigidbody(GameObject* pGameObject)
	: Component(pGameObject)
	, m_mass(1)
	, m_friction(0.1f)
	, m_bounciness(0.8f)
	, m_gravity(Vector3(0.0f, -0.1f, 0.0f))
	, m_isTrigger(false)
	, m_isKinematic(false)
	, m_isGround(false)
{}

Rigidbody::~Rigidbody()
{}

void Rigidbody::Update()
{
	//Transformのみの操作の場合摩擦、重力処理を行わない
	if (!m_isKinematic)
	{
		auto transform = GetGameObject()->GetComponent<Transform>();
		auto pos = transform->GetWorldPosition();

		//力の計算
		if (Utility::VECtoFloat(XMVector3LengthEst(m_force)) > 0)
		{
			//移動力の計算
			Vector3 force = m_force / m_mass;
			m_velocity += force;
			m_force *= 0.9f;
		}

		//摩擦
		Vector3 fricForce = m_velocity * -1;
		fricForce *= m_friction;
		Vector3 fricAcc = fricForce * m_mass;
		if (m_isGround)
			fricAcc *= 2.0f;
		m_velocity += fricAcc;

		pos += m_velocity;
		pos += m_gravity;

		//フィールド以下には行かないように
		auto scene = SceneManager::GetInstance().GetScene();
		MainGame::MeshField* field = scene->GetGameObject<MainGame::MeshField>(scene->OBJECT);
		float fieldHeight = field->GetHeight(pos);
		if (pos.y <= fieldHeight)
		{
			pos.y = fieldHeight;
			m_velocity.y = 0.0f;
			m_isGround = true;
		}
		else
			m_isGround = false;

		transform->SetPosition(pos);
	}
}

void Rigidbody::SetVelocity(Vector3 velocity)
{
	m_velocity = velocity;
}

void Rigidbody::SetVelocityZero()
{
	SetVelocity(Vector3(0, 0, 0));
}

void Rigidbody::SetGravity(Vector3 gravity)
{
	m_gravity = gravity;
}

void Rigidbody::SetGravityZero()
{
	SetGravity(Vector3(0, 0, 0));
}

void Rigidbody::SetMass(float mass)
{
	m_mass = mass;
}

void Rigidbody::AddForce(Vector3 force)
{
	m_force = force;
}