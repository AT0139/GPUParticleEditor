#include "SphereCollision.h"
#include "GameObject.h"
#include "Main.h"
#include "CollisionUtility.h"
#include "Renderer.h"

SphereCollision::SphereCollision(GameObject* pGameObject)
	: CollisionComponent(pGameObject)
	, m_radius(1.0f)
{
#ifdef _DEBUG
	m_model = GetGameObject()->AddComponent<DrawModel>(GetGameObject());
	m_model->Load("asset\\model\\Sphere.obj");
	m_model->SetRasterizerState(RASTERIZER::WIRE_FRAME);
	SetCollisionScale(Vector3(m_radius, m_radius, m_radius));
	m_model->SetCollisionDraw(true);
#endif // _DEBUG
}

void SphereCollision::Update()
{
}

void SphereCollision::Draw()
{
}

SphereInfo SphereCollision::GetSphereInfo()
{
	SphereInfo sphere;

	sphere.center = GetGameObject()->GetComponent<Transform>()->GetWorldPosition();
	sphere.radius = m_radius;

	return sphere;
}

SphereInfo SphereCollision::GetPrevSphereInfo()
{
	SphereInfo sphere;

	sphere.center = GetGameObject()->GetComponent<Transform>()->GetPrevPosition();
	sphere.radius = m_radius;

	return sphere;
}

void SphereCollision::SetRadius(float rad)
{
	m_radius = rad;
	SetCollisionScale(Vector3(m_radius, m_radius, m_radius));
}

void SphereCollision::CollisionBridge(const std::shared_ptr<CollisionComponent>& opponent)
{
	//ダブルディスパッチ
	opponent->HitTest(*this);
}

void SphereCollision::HitTest(SphereCollision& opponent)
{
	auto mySphere = GetSphereInfo();
	auto oppSphere = opponent.GetSphereInfo();

	if (CollisionUtility::SphereSphere(mySphere, oppSphere))
	{
		//当たっている
		CollisonAfter(this, &opponent);
	}
}

void SphereCollision::HitTest(AABBCollision& opponent)
{
	auto mySphere = GetSphereInfo();
	auto oppAABB = opponent.GetAABBInfo();

	if (CollisionUtility::AABBSphere(oppAABB, mySphere))
	{
		//当たっている
		CollisonAfter(this, &opponent);
	}
}

void SphereCollision::HitTest(OBBCollision& opponent)
{
	if (CollisionUtility::ObbSphere(opponent.GetOBBInfo(), GetSphereInfo()))
	{
		CollisonAfter(this, &opponent);
	}
}