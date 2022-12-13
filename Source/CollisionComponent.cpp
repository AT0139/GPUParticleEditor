#include "CollisionComponent.h"
#include "SphereCollision.h"
#include "Rigidbody.h"
#include "GameObject.h"

CollisionComponent::CollisionComponent(GameObject* pGameObject)
	: Component(pGameObject)
	, m_isStaticObject(false)
	, m_collisionScale(Vector3(1.0f,1.0f,1.0f))
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

bool CollisionComponent::IsParent(GameObject* obj)
{
	GameObject* parent = GetGameObject()->GetComponent<Transform>()->GetParent();
	while (parent != nullptr)
	{
		if (parent == obj)
			return true;
		parent = parent->GetComponent<Transform>()->GetParent();
	}
	return false;
}

void CollisionComponent::CollisionReset()
{
	m_hitObjects.clear();
}

void CollisionComponent::CollisonAfter(CollisionComponent* col1, CollisionComponent* col2, Vector3 hitNormal)
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

	Vector3 slide = Slide(myRigidbody->GetVelocity(), hitNormal);
	myTransform->SetToPrev();
	auto WorldPos = myTransform->GetWorldPosition() + slide;
	auto hit = hitNormal;
	WorldPos += hitNormal * (-1.0f * 0.5f);
	myTransform->SetWorldPosition(WorldPos);


	if (!myRigidbody->GetIsTrigger() && !oppRigidbody->GetIsTrigger())
	{
		float ResultPower = -(1.0f + myRigidbody->GetBounciness());
		if (oppRigidbody)
		{
			Vector3 RelativeVelo = myRigidbody->GetVelocity() - oppRigidbody->GetVelocity();
			ResultPower = (-(1.0f + myRigidbody->GetBounciness()) * RelativeVelo.Dot(hitNormal)) /
				((hitNormal.Dot(hitNormal)) * (1 / myRigidbody->GetMass() + 1 / oppRigidbody->GetMass()));
		}
		else
		{
			Vector3 RelativeVelo = myRigidbody->GetVelocity();
			ResultPower = (-(1.0f + myRigidbody->GetBounciness()) * RelativeVelo.Dot(hitNormal)) /
				(hitNormal.Dot(hitNormal) * (1 / myRigidbody->GetMass()));
		}
		auto Velo = myRigidbody->GetVelocity();
		Velo += (hitNormal * ResultPower) / myRigidbody->GetMass();
		myRigidbody->SetVelocity(Velo);

		////反発
		//Vector3 myForce = ((myMass - oppRigidbody->GetBounciness() * oppMass) * myRigidbody->GetVelocity() + (1 + oppRigidbody->GetBounciness()) * oppMass * oppRigidbody->GetVelocity()) / (oppMass + myMass);
		//myForce *= 0.5f;
		//myRigidbody->AddForce(myForce);

		//Vector3 oppForce = ((1 + myRigidbody->GetBounciness()) * myMass * myRigidbody->GetVelocity() + (oppMass - myRigidbody->GetBounciness() * myMass) * oppRigidbody->GetVelocity()) / (oppMass + myMass);
		//oppForce *= 0.5f;
		//oppRigidbody->AddForce(oppForce);
	}
	//todo;ここ違う
	if (!myRigidbody->GetIsTrigger())
	{
		//衝突関数の呼び出し
		oppGameObj->OnCollision(myGameObj);
	}
	else
	{
		//衝突関数の呼び出し
		oppGameObj->OnTrigger(myGameObj);
	}

	if (!oppRigidbody->GetIsTrigger())
	{
		//衝突関数の呼び出し
		myGameObj->OnCollision(oppGameObj);
	}
	else
	{
		//衝突関数の呼び出し
		myGameObj->OnTrigger(oppGameObj);
	}
}

PlaneInfo AABBInfo::GetPlane(GameObject* gameObject)
{

	//面のベクトル
	Vector3 point0 = center;
	float MakedHalfX = scaleHalf.x;
	float MakedHalfY = scaleHalf.y;
	//2つ目は-0.5,-0.5,0の点をワールド変換したもの
	Vector3 point1(-MakedHalfX, -MakedHalfY, 0);
	auto matrix = gameObject->GetComponent<Transform>()->GetWorldMatrix();
	point1 = XMVector3Transform(point1, matrix);
	//3つ目は-0.5,0.5,0の点をワールド変換したもの
	Vector3 point2(MakedHalfX, -MakedHalfY, 0);
	point2 = XMVector3Transform(point2, matrix);
	//3点を使って面を作成
	PlaneInfo ret;
	ret.normal = point1 - point0;
	ret.normal.Dot(point2 - point0);
	ret.normal.Normalize();
	ret.dotVal = ret.normal.Dot(point0);
	return ret;
}
