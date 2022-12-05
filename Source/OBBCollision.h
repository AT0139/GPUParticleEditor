#pragma once

#include "CollisionComponent.h"

class OBBCollision : public CollisionComponent
{
public:
	OBBCollision(GameObject* pGameObject);
	~OBBCollision();

	// Component を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;


	// CollisionComponent を介して継承されました
	virtual void CollisionBridge(const std::shared_ptr<CollisionComponent>& opponent) override;
	virtual void HitTest(SphereCollision& opponent) override;
	virtual void HitTest(AABBCollision& opponent) override;
	virtual void HitTest(OBBCollision& opponent) override;

	OBBInfo GetOBBInfo();

	void SetCollisionScale(Vector3 scale);
	void SetCollisionScale(float scale);


private:
	Vector3 m_collisionScale;
#ifdef _DEBUG
	std::shared_ptr<DrawModel> m_model;
#endif 
};