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
		//当たっている
		
	}
}

void SphereCollision::CollisionBridge(const std::shared_ptr<CollisionComponent>& opponent)
{
	//ダブルディスパッチ
	opponent->HitTest(*this);
}