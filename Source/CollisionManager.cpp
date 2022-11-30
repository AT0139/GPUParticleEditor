#include "CollisionManager.h"
#include "CollisionComponent.h"
#include "Manager.h"
#include "Scene.h"

void CollisionManager::Update()
{
	m_collisionVector.clear();
	auto& objects = Manager::GetInstance().GetScene()->GetAllObject();
	for (auto& obj : objects)
	{
		auto pCollision = obj->GetComponent<CollisionComponent>();
		if (pCollision)
		{
			//必要な情報を格納
			CollisionInfo colInfo;
			colInfo.collision = pCollision;
			m_collisionVector.push_back(colInfo);
		}
	}

	for (int i = 0; i < m_collisionVector.size(); i++)
	{
		Collision(i);
	}

	
	for (auto obj : m_collisionVector)
	{
		obj.collision->CollisionReset();
	}
}

void CollisionManager::Collision(int index)
{
	CollisionInfo& rec = m_collisionVector[index];

	for (auto& col : m_collisionVector)
	{
		if (rec == col)
			continue;

		//? ここに除外するオブジェクトを追加
		if (col.collision->IsHitObject(rec.collision->GetGameObject()))
			continue;

		col.collision->CollisionBridge(rec.collision);
	}
}