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
	auto mySphere = GetSphereInfo();
	auto oppSphere = opponent.GetSphereInfo();

	if (CollisionUtility::SphereSphere(mySphere, oppSphere))
	{
		//当たっている
		auto myGameObj = this->GetGameObject();
		auto oppGameObj = opponent.GetGameObject();

		//衝突相手の登録
		AddHitObject(*oppGameObj);
		if (!opponent.IsStaticObject())
			opponent.AddHitObject(*myGameObj);

		//衝突関数の呼び出し
		myGameObj->OnCollision(oppGameObj);
		oppGameObj->OnCollision(oppGameObj);
	}
}

void SphereCollision::CollisionBridge(const std::shared_ptr<CollisionComponent>& opponent)
{
	//ダブルディスパッチ
	opponent->HitTest(*this);
}