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

	virtual SphereInfo GetConnectSphere() = 0;
	virtual void BackToBefore(const D3DXVECTOR3 TotalVelocoty, float SpanTime) = 0;
	virtual void CollisionEscape(SphereCollision& DestColl) = 0;

	virtual void CollisionBridge(const std::shared_ptr<CollisionComponent>& opponent) = 0;
	virtual void HitTest(SphereCollision& opponent) = 0;
	virtual void GetHitNormal(SphereCollision& DestColl, D3DXVECTOR3& Ret) = 0;

	void CollisionReset();

	template<typename SrcType, typename DestType>
	void AfterCollisionTmpl_1(const D3DXVECTOR3& SrcVelocity, const D3DXVECTOR3& DestVelocity, float ElapsedTime, float HitTime, SrcType& SrcColl, DestType& DestColl)
	{
		if (HitTime <= 0) {
			HitTime = 0;
		}
		if (HitTime >= ElapsedTime) {
			HitTime = ElapsedTime;
		}
		auto AfterHitTime = ElapsedTime - HitTime;
		bool UseSrcAfter = false;
		bool UseDestAfter = false;
		//衝突相手の登録
		AddHitObject(*DestColl.GetGameObject());
		if (GetHitAction() != HitAction::None)
		{
			//事後処理が有効なら衝突前まで戻る
			BackToBefore(SrcVelocity, HitTime);
			UseSrcAfter = true;
		}
		if (!DestColl.IsStaticObject())
		{
			//もし相手がFixでなければ相手の衝突相手の登録
			//DestColl.AddHitObject(*GetGameObject());
			if (DestColl.GetHitAction() != HitAction::None)
			{
				//事後処理が有効なら衝突前まで戻る
				DestColl.BackToBefore(DestVelocity, HitTime);
				UseDestAfter = true;
			}
		}
		else
		{
			//相手がFixなら自己退避だけでは衝突している可能性があるので判定をしてエスケープ処理
			if (GetHitAction() != HitAction::None)
			{
				CollisionEscape(DestColl);
			}
		}
		if (UseSrcAfter)
		{
			D3DXVECTOR3 HitNormal;
			GetHitNormal(DestColl, HitNormal);
			HittingProcess(DestColl, SrcVelocity, DestVelocity, HitNormal, AfterHitTime);
		}
		if (UseDestAfter)
		{
			D3DXVECTOR3 HitNormal;
			DestColl.GetHitNormal(SrcColl, HitNormal);
			DestColl.HittingProcess(DestColl, SrcVelocity, DestVelocity, HitNormal, AfterHitTime);
		}
	}

protected:
	void HittingProcess(const CollisionComponent& DestColl, const D3DXVECTOR3& SrcVelocity, const D3DXVECTOR3& DestVelocity,
		D3DXVECTOR3 HitNormal, float AfterHitTime);

private:
	void AddHitObject(GameObject& obj) { m_hitObjects.push_back(&obj); }
	HitAction GetHitAction()const { return m_hitAction; }
	D3DXVECTOR3 Slide(const D3DXVECTOR3& Vec, const D3DXVECTOR3& Norm);
	std::vector<GameObject*> m_hitObjects;
	bool m_isStaticObject;
	HitAction m_hitAction;
};