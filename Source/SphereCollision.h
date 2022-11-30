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
	virtual void GetHitNormal(SphereCollision& DestColl, D3DXVECTOR3& Ret) override;

	// CollisionComponent を介して継承されました
	virtual SphereInfo GetConnectSphere() override;
	virtual void HitTest(SphereCollision& opponent) override;
	virtual void CollisionBridge(const std::shared_ptr<CollisionComponent>& opponent) override;
	virtual void BackToBefore(const D3DXVECTOR3 TotalVelocoty, float SpanTime) override;
	virtual void CollisionEscape(SphereCollision& DestColl) override;
};