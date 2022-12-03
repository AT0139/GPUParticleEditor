#pragma once

#include "CollisionComponent.h"

class AABBCollision : public CollisionComponent
{
public:
	AABBCollision(GameObject* pGameObject);
	~AABBCollision();

	// Component を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

	void SetCollisionScale(D3DXVECTOR3 scale);

	// CollisionComponent を介して継承されました
	virtual void CollisionBridge(const std::shared_ptr<CollisionComponent>& opponent) override;
	virtual void HitTest(SphereCollision& opponent) override;
	virtual void HitTest(AABBCollision& opponent) override;

	AABBInfo GetAABBInfo();
private:
	D3DXVECTOR3 m_collisionScale;
#ifdef _DEBUG
	std::shared_ptr<DrawModel> m_model;
#endif 
};