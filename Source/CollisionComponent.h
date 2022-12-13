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

struct PlaneInfo
{
	Vector3 normal;
	float dotVal;
};

struct AABBInfo
{
	Vector3 center;
	Vector3 scaleHalf;

	PlaneInfo GetPlane(GameObject* gameObject);
};

struct OBBInfo
{
	Vector3 center;
	Vector3 X,Y,Z;
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

	virtual Vector3 GetHitNormal(SphereCollision& opponent) = 0;
	virtual Vector3 GetHitNormal(AABBCollision& opponent) = 0;
	virtual Vector3 GetHitNormal(OBBCollision& opponent) = 0;
	virtual Vector3 GetHitNormal(CapsuleCollision& opponent) = 0;

	bool IsHitObject(GameObject* obj);
	bool IsParent(GameObject* obj);
	void CollisionReset();

	void AddHitObject(GameObject& obj) { m_hitObjects.push_back(&obj); }
	Vector3 GetCollisionScale() { return m_collisionScale; }
protected:
	void CollisonAfter(CollisionComponent* col1, CollisionComponent* col2, Vector3 hitNormal);
	void SetCollisionScale(Vector3 scale) { m_collisionScale = scale;}

	template<typename MyType,typename OppType>
	void AfterCollisionTemplate(MyType* myCol, OppType* oppCol)
	{
		//衝突相手の登録
		AddHitObject(*oppCol->GetGameObject());
		oppCol->AddHitObject(*myCol->GetGameObject());

		//todo:動かないオブジェクトの追加
		//衝突処理
		{
			Vector3 hitNormal = GetHitNormal(*oppCol);
			CollisonAfter(oppCol, myCol, hitNormal);
		}
		{
			Vector3 hitNormal = oppCol->GetHitNormal(*myCol);
			oppCol->CollisonAfter(myCol, oppCol, hitNormal);
		}
	}

private:
	inline  Vector3 Slide(const Vector3& Vec, const Vector3& Norm)
	{
		//thisと法線から直行線の長さ（内積で求める）
		float Len = Vec.Dot(Norm);
		//その長さに伸ばす
		Vector3 Contact = Norm * Len;
		//スライドする方向は現在のベクトルから引き算
		return (Vec - Contact);
	}

	std::vector<GameObject*> m_hitObjects;
	bool m_isStaticObject;
	Vector3 m_collisionScale;
};