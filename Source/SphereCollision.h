#pragma once

#include "CollisionComponent.h"

class SphereCollision : public CollisionComponent
{
public:
	SphereCollision(GameObject* pGameObject);
	~SphereCollision() {}

	// Component を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

	SphereInfo GetSphereInfo();
	SphereInfo GetPrevSphereInfo();

	// CollisionComponent を介して継承されました
	virtual void HitTest(SphereCollision& opponent) override;
	virtual void CollisionBridge(const std::shared_ptr<CollisionComponent>& opponent) override;
};