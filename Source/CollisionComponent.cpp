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

	//衝突相手の登録
	AddHitObject(*oppGameObj);
	if (!col2->IsStaticObject())
		col2->AddHitObject(*myGameObj);

	//衝突関数の呼び出し
	myGameObj->OnCollision(oppGameObj);
	oppGameObj->OnCollision(oppGameObj);
}
