#include "CollisionComponent.h"
#include "SphereCollision.h"
#include "Rigidbody.h"
#include "GameObject.h"

CollisionComponent::CollisionComponent(GameObject* pGameObject)
	: Component(pGameObject)
	, m_isStaticObject(false)
{}

void CollisionComponent::HittingProcess(const CollisionComponent& opponent, D3DXVECTOR3 normal)
{
	auto myTransform = GetGameObject()->GetComponent<Transform>();
	

	auto myRigid = GetGameObject()->GetComponent<Rigidbody>();

	if (myRigid)
	{
		D3DXVECTOR3 SrcVelocity = GetGameObject()->GetComponent<Transform>()->GetVelocity();
		D3DXVECTOR3 DestVelocity = opponent.GetGameObject()->GetComponent<Transform>()->GetVelocity();

		//反発
		auto oppRigid = opponent.GetGameObject()->GetComponent<Rigidbody>();
		float power = -(1.0f + myRigid->GetBounciness());
		if (oppRigid)
		{
			D3DXVECTOR3 RelativeVelo = SrcVelocity - DestVelocity;
			power = (-(1.0f + myRigid->GetBounciness()) * D3DXVec3Dot(&RelativeVelo, &normal)) /
				(D3DXVec3Dot(&normal, &normal) * (1 / myRigid->GetMass() + 1 / oppRigid->GetMass()));
		}
		else
		{
			D3DXVECTOR3 RelativeVelo = SrcVelocity;
			power = (-(1.0f + myRigid->GetBounciness()) * D3DXVec3Dot(&RelativeVelo, &normal)) /
				(D3DXVec3Dot(&normal, &normal) * (1 / myRigid->GetMass()));
		}
		auto velocity = myRigid->GetVelocity();
		velocity += (normal * power) / myRigid->GetMass();
		myRigid->SetVelocity(velocity);
	}
}

bool Collision::SphereSphere(SphereInfo Sphere1, SphereInfo Sphere2)
{

	D3DXVECTOR3 dir = Sphere1.center - Sphere2.center;
	float dist = D3DXVec3Dot(&dir, &dir);
	float rad = Sphere1.radius + Sphere2.radius;

	return dist < rad * rad;
}