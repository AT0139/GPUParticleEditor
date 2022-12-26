#include "CapsuleCollision.h"
#include "GameObject.h"
#include "CollisionUtility.h"
#include "DrawModel.h"
#include "Renderer.h"

CapsuleCollision::CapsuleCollision(GameObject* pGameObject)
	: CollisionComponent(pGameObject)
{
#ifdef _DEBUG
	m_model = GetGameObject()->AddComponent<DrawModel>(GetGameObject());
	m_model->Load("asset\\model\\Cube.obj");
	m_model->SetRasterizerState(RASTERIZER::WIRE_FRAME);
	m_model->SetCollisionDraw(true);
#endif // _DEBUG
}

CapsuleCollision::~CapsuleCollision()
{
}

void CapsuleCollision::Update()
{
}

void CapsuleCollision::Draw()
{
}

void CapsuleCollision::CollisionBridge(const std::shared_ptr<CollisionComponent>& opponent)
{
	//ダブルディスパッチ
	opponent->HitTest(*this);
}

void CapsuleCollision::HitTest(CapsuleCollision& opponent)
{
	//if (CollisionUtility::CapsuleCapsule(GetCapsuleInfo(), opponent.GetCapsuleInfo()))
	//{
	//	CollisonAfter(this, &opponent);
	//}
}

void CapsuleCollision::HitTest(SphereCollision& opponent)
{
	//if (CollisionUtility::ObbSphere(GetCapsuleInfo(), opponent.GetSphereInfo()))
	//{
	//	CollisonAfter(this, &opponent);
	//}
}

void CapsuleCollision::HitTest(AABBCollision& opponent)
{
	//if (CollisionUtility::ObbAabb(GetCapsuleInfo(), opponent.GetAABBInfo()))
	//{
	//	CollisonAfter(this, &opponent);
	//}
}

void CapsuleCollision::HitTest(OBBCollision& opponent)
{
	//if (CollisionUtility::ObbObb(GetCapsuleInfo(), opponent.GetOBBInfo()))
	//{
	//	CollisonAfter(this, &opponent);
	//}
}

CapsuleInfo CapsuleCollision::GetCapsuleInfo()
{
	CapsuleInfo cap;

	cap.radius = m_radius;

	return cap;
}

void CapsuleCollision::SetRadius(float rad)
{
	m_radius = rad;
}

Vector3 CapsuleCollision::GetHitNormal(SphereCollision& opponent)
{
	return Vector3();
}

Vector3 CapsuleCollision::GetHitNormal(AABBCollision& opponent)
{
	return Vector3();
}

Vector3 CapsuleCollision::GetHitNormal(OBBCollision& opponent)
{
	return Vector3();
}

Vector3 CapsuleCollision::GetHitNormal(CapsuleCollision& opponent)
{
	return Vector3();
}