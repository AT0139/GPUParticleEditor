#pragma once

#include "Component.h"
#include "DrawModel.h"

class SphereCollision;
class AABBCollision;

struct SphereInfo
{
	D3DXVECTOR3 center;
	float radius;
};

struct AABBInfo
{
	D3DXVECTOR3 center;
	D3DXVECTOR3 scaleHalf;
};

class CollisionComponent : public Component
{
public:
	CollisionComponent(GameObject* pGameObject);
	virtual ~CollisionComponent() {}

	// Component を介して継承されました
	virtual void Update() override {}
	virtual void Draw() override {}

	bool IsStaticObject()const { return m_isStaticObject; }

	virtual void CollisionBridge(const std::shared_ptr<CollisionComponent>& opponent) = 0;
	virtual void HitTest(SphereCollision& opponent) = 0;
	virtual void HitTest(AABBCollision& opponent) = 0;
	
	bool IsHitObject(GameObject* obj);
	void CollisionReset();

	void AddHitObject(GameObject& obj) { m_hitObjects.push_back(&obj); }

protected:
	void CollisonAfter(CollisionComponent* col1, CollisionComponent* col2);

private:
	std::vector<GameObject*> m_hitObjects;
	bool m_isStaticObject;
};