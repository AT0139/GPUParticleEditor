#include "CollisionComponent.h"
#include "SphereCollision.h"
#include "Rigidbody.h"
#include "GameObject.h"

CollisionComponent::CollisionComponent(GameObject* pGameObject)
	: Component(pGameObject)
	, m_isStaticObject(false)
	, m_hitAction(HitAction::Auto)
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

