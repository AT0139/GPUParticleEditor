#pragma once

#include "Component.h"
#include "DrawModel.h"

class SphereCollision;
class AABBCollision;
class OBBCollision;
class CapsuleCollision;

struct LineInfo
{
	Vector3 start;
	Vector3 end;
};

struct SphereInfo
{
	Vector3 center;
	float radius;
};

struct AABBInfo
{
	Vector3 center;
	Vector3 scaleHalf;
};

struct OBBInfo
{
	Vector3 center;
	Vector3 X, Y, Z;
	Vector3 scaleHalf;
};

struct CapsuleInfo
{
	LineInfo line;
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

	bool IsStaticObject()const { return m_isStaticObject; }

	virtual void CollisionBridge(const std::shared_ptr<CollisionComponent>& opponent) = 0;
	virtual void HitTest(SphereCollision& opponent) = 0;
	virtual void HitTest(AABBCollision& opponent) = 0;
	virtual void HitTest(OBBCollision& opponent) = 0;
	virtual void HitTest(CapsuleCollision& opponent) = 0;

	bool IsHitObject(GameObject* obj);
	bool IsParent(GameObject* obj);
	void CollisionReset();

	void AddHitObject(GameObject& obj) { m_hitObjects.push_back(&obj); }
	Vector3 GetCollisionScale() { return m_collisionScale; }
protected:
	void CollisonAfter(CollisionComponent* col1, CollisionComponent* col2);
	void SetCollisionScale(Vector3 scale) { m_collisionScale = scale;}

private:
	std::vector<GameObject*> m_hitObjects;
	bool m_isStaticObject;
	Vector3 m_collisionScale;
};