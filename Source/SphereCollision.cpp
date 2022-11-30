#include "SphereCollision.h"
#include "GameObject.h"
#include "Utility.h"
#include "Main.h"
#include "CollisionUtility.h"

SphereCollision::SphereCollision(GameObject* pGameObject)
	: CollisionComponent(pGameObject)
{}

void SphereCollision::Update()
{
}

void SphereCollision::Draw()
{
}

SphereInfo SphereCollision::GetSphereInfo()
{
	SphereInfo sphere;

	sphere.center = GetGameObject()->GetComponent<Transform>()->GetPosition();
	sphere.radius = 1;

	return sphere;
}

SphereInfo SphereCollision::GetPrevSphereInfo()
{
	SphereInfo sphere;

	sphere.center = GetGameObject()->GetComponent<Transform>()->GetPrevPosition();
	sphere.radius = 1;

	return sphere;
}

SphereInfo SphereCollision::GetConnectSphere()
{
	return SphereInfo();
}

void SphereCollision::HitTest(SphereCollision& opponent)
{
	auto ptrTransform = GetGameObject()->GetComponent<Transform>();
	auto destTransform = opponent.GetGameObject()->GetComponent<Transform>();
	D3DXVECTOR3 srcVelocity = ptrTransform->GetVelocity();
	D3DXVECTOR3 destVelocity = destTransform->GetVelocity();

	//前回のターンからの時間
	float ElapsedTime = FPS;
	//球の場合は、すべて移動以外変化なしとする
	SphereInfo SrcSphere = GetSphereInfo();
	SphereInfo SrcBeforSphere = GetPrevSphereInfo();
	//相手のCollisionSphere
	SphereInfo DestSphere = opponent.GetSphereInfo();
	SphereInfo DestBeforeSphere = opponent.GetPrevSphereInfo();
	D3DXVECTOR3 SpanVelocity = srcVelocity - destVelocity;
	float HitTime = 0;

	if (CollisionUtility::TestSphereSphere(SrcBeforSphere, SpanVelocity, DestBeforeSphere, 0, ElapsedTime, HitTime))
	{
		AfterCollisionTmpl_1< SphereCollision, SphereCollision>
			(srcVelocity, destVelocity, ElapsedTime, HitTime, *this, opponent);
	}
}

void SphereCollision::CollisionBridge(const std::shared_ptr<CollisionComponent>& opponent)
{
	//ダブルディスパッチ
	opponent->HitTest(*this);
}

void SphereCollision::BackToBefore(const D3DXVECTOR3 TotalVelocoty, float SpanTime)
{
	//すべて移動以外変化なしとする
	SphereInfo SrcSphere = GetSphereInfo();
	SphereInfo SrcBeforSphere = GetPrevSphereInfo();
	auto PtrTransform = GetGameObject()->GetComponent<Transform>();
	D3DXVECTOR3 Pos = SrcBeforSphere.center + TotalVelocoty;
	PtrTransform->SetPosition(Pos);
}

void SphereCollision::CollisionEscape(SphereCollision& DestColl)
{
	SphereInfo SrcSphere = GetSphereInfo();
	SphereInfo DestSphere = DestColl.GetSphereInfo();
	if (CollisionUtility::SphereSphere(SrcSphere, DestSphere)) {
		D3DXVECTOR3 Normal = SrcSphere.center - DestSphere.center;
		D3DXVec3Normalize(&Normal, &Normal);
		float Span = SrcSphere.radius + DestSphere.radius;
		Normal *= Span;
		auto PtrTransform = GetGameObject()->GetComponent<Transform>();
		D3DXVECTOR3 Pos = DestSphere.center + Normal;
		//エスケープはリセット
		PtrTransform->SetPosition(Pos);
	}
}

void SphereCollision::GetHitNormal(SphereCollision& DestColl, D3DXVECTOR3& Ret)
{
	SphereInfo sp = GetSphereInfo();
	SphereInfo sp2 = DestColl.GetSphereInfo();
	//接点へのベクトル
	Ret = sp2.center - sp.center;
	D3DXVec3Normalize(&Ret, &Ret);
}