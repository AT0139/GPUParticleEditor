#include "CollisionComponent.h"
#include "SphereCollision.h"
#include "Rigidbody.h"
#include "GameObject.h"

CollisionComponent::CollisionComponent(GameObject* pGameObject)
	: Component(pGameObject)
	, m_isStaticObject(false)
{}

bool CollisionComponent::IsHitObject(GameObject* obj)
{
	if (!m_hitObjects.empty())	
	{
		for (auto object : m_hitObjects)
		{
			if (object == obj)
			{
				return true;
			}
		}
	}
	return false;
}

void CollisionComponent::CollisionReset()
{
	m_hitObjects.clear();
}

void CollisionComponent::CollisonAfter(CollisionComponent* col1, CollisionComponent* col2)
{
	//当たっている
	auto myGameObj = col1->GetGameObject();
	auto oppGameObj = col2->GetGameObject();
	auto myTransform = myGameObj->GetComponent<Transform>();
	auto oppTransform = oppGameObj->GetComponent<Transform>();
	auto myRigidbody = myGameObj->GetComponent<Rigidbody>();
	auto oppRigidbody = oppGameObj->GetComponent<Rigidbody>();

	float myMass = myRigidbody->GetMass();
	float oppMass = oppRigidbody->GetMass();

	//衝突相手の登録
	AddHitObject(*oppGameObj);
	if (!col2->IsStaticObject())
		col2->AddHitObject(*myGameObj);

	//衝突関数の呼び出し
	myGameObj->OnCollision(oppGameObj);
	oppGameObj->OnCollision(oppGameObj);

	//前回位置に戻す
	//myTransform->SetPosition(myTransform->GetPrevPosition());
	//oppTransform->SetPosition(oppTransform->GetPrevPosition());
	D3DXVECTOR3 myForce = ((myMass - oppRigidbody->GetBounciness() * oppMass) * myRigidbody->GetVelocity() + (1 + oppRigidbody->GetBounciness())*oppMass * oppRigidbody->GetVelocity()) / (oppMass + myMass);
	D3DXVECTOR3 oppForce = ((1 + myRigidbody->GetBounciness()) * myMass * myRigidbody->GetVelocity() + (oppMass - myRigidbody->GetBounciness() * myMass) * oppRigidbody->GetVelocity()) / (oppMass + myMass);
	myForce *= 0.5f;
	oppForce *= 0.5f;

	myRigidbody->AddForce(myForce);
	oppRigidbody->AddForce(oppForce);
}