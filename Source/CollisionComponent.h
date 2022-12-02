#pragma once

#include "Component.h"

class SphereCollision;

struct SphereInfo
{
	D3DXVECTOR3 center;
	float radius;
};

enum class HitAction
{
	None,
	Slide,
	Auto,
	Stop,
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
	
	bool IsHitObject(GameObject* obj);
	void CollisionReset();

private:
	void AddHitObject(GameObject& obj) { m_hitObjects.push_back(&obj); }
	HitAction GetHitAction()const { return m_hitAction; }

	std::vector<GameObject*> m_hitObjects;
	bool m_isStaticObject;
	HitAction m_hitAction;
};