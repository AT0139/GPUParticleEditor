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
	void SetRadius(float rad);

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
private:
	float m_radius;
#ifdef _DEBUG
	std::shared_ptr<DrawModel> m_model;
#endif
};