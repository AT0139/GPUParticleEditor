#include "Rigidbody.h"
#include "Transform.h"
#include "GameObject.h"
#include "Scene.h"
#include "Manager.h"
#include "MeshField.h"


static const float GRAVITY = 1.0f;

Rigidbody::Rigidbody(GameObject* pGameObject)
	: Component(pGameObject)
	, m_mass(1)
	, m_friction(0.1f)
	, m_bounciness(0.0f)
	, m_gravity(D3DXVECTOR3(0.0f,-0.01f,0.0f))
{}

Rigidbody::~Rigidbody()
{}

void Rigidbody::Update()
{
	auto transform = GetGameObject()->GetComponent<Transform>();
	auto pos = transform->GetWorldPosition();

	//力の計算
	if (D3DXVec3Length(&m_force) > 0)
	{
		//移動力の計算
		D3DXVECTOR3 force = m_force / m_mass;
		m_velocity += force;
		m_force = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		
	}

	//摩擦
	D3DXVECTOR3 fricForce = m_velocity * -1;
	fricForce *= m_friction;
	D3DXVECTOR3 fricAcc = fricForce * m_mass;
	m_velocity += fricAcc;

	pos += m_velocity;
	pos += m_gravity;

	auto scene = Manager::GetInstance().GetScene();
	MainGame::MeshField* field = scene->GetGameObject<MainGame::MeshField>(scene->OBJECT);
	pos.y = field->GetHeight(pos);
	transform->SetPosition(pos);
}

void Rigidbody::SetVelocity(D3DXVECTOR3 velocity)
{
	m_velocity = velocity;
}

void Rigidbody::SetVelocityZero()
{
	SetVelocity(D3DXVECTOR3(0, 0, 0));
}

void Rigidbody::SetGravity(D3DXVECTOR3 gravity)
{
	m_gravity = gravity;
}

void Rigidbody::SetGravityZero()
{
	SetGravity(D3DXVECTOR3(0, 0, 0));
}

void Rigidbody::SetMass(float mass)
{
	m_mass = mass;
}

void Rigidbody::AddForce(D3DXVECTOR3 force)
{
	m_force = force;
}
