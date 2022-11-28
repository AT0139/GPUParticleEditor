#include "SphereCollision.h"
#include "GameObject.h"
#include "Utility.h"

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

SphereInfo SphereCollision::GetConnectSphere()
{
	return SphereInfo();
}

void SphereCollision::HitTest(SphereCollision& opponent)
{
	auto myTransform = GetGameObject()->GetComponent<Transform>();
	SphereInfo myInfo;
	SphereInfo oppInfo;

	myInfo.center = myTransform->GetPosition();
	//myInfo.radius = GetGameObject()->GetComponent<Transform>()->GetScale().x;
	myInfo.radius = 1;
	oppInfo.center = opponent.GetGameObject()->GetComponent<Transform>()->GetPosition();
	//oppInfo.radius = opponent.GetGameObject()->GetComponent<Transform>()->GetScale().x;
	oppInfo.radius = 1;
	if (Collision::SphereSphere(myInfo, oppInfo))
	{
		D3DXVECTOR3 hitNormal;

		SphereInfo sp = GetSphereInfo();
		SphereInfo sp2 = opponent.GetSphereInfo();
		//接点へのベクトル
		hitNormal = sp2.center - sp.center;
		D3DXVec3Normalize(&hitNormal, &hitNormal);

		HittingProcess(opponent, hitNormal);
	}
}

void SphereCollision::CollisionBridge(const std::shared_ptr<CollisionComponent>& opponent)
{
	//ダブルディスパッチ
	opponent->HitTest(*this);
}
