#pragma once

#include "Component.h"

class SphereCollision;

struct SphereInfo
{
	D3DXVECTOR3 center;
	float radius;
};

class CollisionComponent : public Component
{
public:
	CollisionComponent(GameObject* pGameObject);
	virtual ~CollisionComponent() {}

	// Component を介して継承されました
	virtual void Update() override {}
	virtual void Draw() override {}


	virtual SphereInfo GetConnectSphere() = 0;

	virtual void CollisionBridge(const std::shared_ptr<CollisionComponent>& opponent) = 0;
	virtual void HitTest(SphereCollision& opponent) = 0;
protected:
	void HittingProcess(const CollisionComponent& opponent, D3DXVECTOR3 normal);

	bool m_isStaticObject;
};

namespace Collision
{
	bool SphereSphere(SphereInfo Sphere1, SphereInfo Sphere2);
};