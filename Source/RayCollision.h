#pragma once

#include "CollisionComponent.h"

class RayCollision : public CollisionComponent
{
public:
	RayCollision(GameObject* pGameObject);
	~RayCollision();

	// Component を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

	// CollisionComponent を介して継承されました
	virtual void CollisionBridge(const std::shared_ptr<CollisionComponent>& opponent) override;
	virtual void HitTest(SphereCollision& opponent) override;
	virtual void HitTest(AABBCollision& opponent) override;
	virtual void HitTest(OBBCollision& opponent) override;
	virtual void HitTest(CapsuleCollision& opponent) override;

	virtual Vector3 GetHitNormal(SphereCollision& opponent) override;
	virtual Vector3 GetHitNormal(AABBCollision& opponent) override;
	virtual Vector3 GetHitNormal(OBBCollision& opponent) override;
	virtual Vector3 GetHitNormal(CapsuleCollision& opponent) override;

	virtual void CollisionEscape(SphereCollision& opponent) override;
	virtual void CollisionEscape(AABBCollision& opponent) override;
	virtual void CollisionEscape(OBBCollision& opponent) override;
	virtual void CollisionEscape(CapsuleCollision& opponent) override;

	RayInfo GetRayInfo();

private:
#ifdef _DEBUG
	std::shared_ptr<DrawModel> m_model;
#endif

	RayInfo m_info;
};