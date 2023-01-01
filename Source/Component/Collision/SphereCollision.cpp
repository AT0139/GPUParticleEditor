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

	sphere.center = GetGameObject()->GetComponent<Transform>()->GetWorldPosition() + m_centerPosition;
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

void SphereCollision::SetScale(float rad)
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
		AfterCollisionTemplate(this, &opponent);
	}
}

void SphereCollision::HitTest(AABBCollision& opponent)
{
	auto mySphere = GetSphereInfo();
	auto oppAABB = opponent.GetAABBInfo();

	if (CollisionUtility::AABBSphere(oppAABB, mySphere))
	{
		//当たっている
		AfterCollisionTemplate(this, &opponent);
	}
}

void SphereCollision::HitTest(OBBCollision& opponent)
{
	Vector3 temp;
	if (CollisionUtility::ObbSphere(opponent.GetOBBInfo(), GetSphereInfo(), temp))
	{
		AfterCollisionTemplate(this, &opponent);
	}
}

void SphereCollision::HitTest(CapsuleCollision& opponent)
{
}

void SphereCollision::HitTest(RayCollision& opponent)
{
}

Vector3 SphereCollision::GetHitNormal(SphereCollision& opponent)
{
	SphereInfo sphere = GetSphereInfo();
	SphereInfo oppSphere = opponent.GetSphereInfo();
	//接点へのベクトル
	Vector3 normal = sphere.center - oppSphere.center;
	normal.Normalize();

	return normal;
}

Vector3 SphereCollision::GetHitNormal(AABBCollision& opponent)
{
	SphereInfo sp = GetSphereInfo();
	AABBInfo aabb = opponent.GetAABBInfo();
	PlaneInfo ret = aabb.GetPlane(opponent.GetGameObject());
	Vector3 returnNormal = ret.normal;
	returnNormal.Normalize();
	return returnNormal;
}

Vector3 SphereCollision::GetHitNormal(OBBCollision& opponent)
{
	SphereInfo sp = GetSphereInfo();
	OBBInfo m_obb = opponent.GetOBBInfo();
	Vector3 normal;
	CollisionUtility::ObbSphere(m_obb, sp, normal);
	//接点へのベクトル
	normal -= sp.center;
	normal.Normalize();
	return normal;
}

Vector3 SphereCollision::GetHitNormal(CapsuleCollision& opponent)
{
	return Vector3(0, 0, 0);
}

Vector3 SphereCollision::GetHitNormal(RayCollision& opponent)
{
	return Vector3();
}

void SphereCollision::CollisionEscape(SphereCollision& opponent)
{
	auto mySphere = this->GetSphereInfo();
	auto oppSphere = opponent.GetSphereInfo();

	Vector3 normal = mySphere.center - oppSphere.center;
	normal.Normalize();
	float Span = mySphere.radius + oppSphere.radius;
	normal *= Span;
	auto transform = GetGameObject()->GetComponent<Transform>();
	Vector3 pos = oppSphere.center + normal;

	transform->SetWorldPosition(pos);
}

void SphereCollision::CollisionEscape(AABBCollision& opponent)
{
}

void SphereCollision::CollisionEscape(OBBCollision& opponent)
{
	auto sphere = GetSphereInfo();
	auto m_obb = opponent.GetOBBInfo();
	Vector3 ret;
	bool hit = CollisionUtility::ObbSphere(m_obb, sphere, ret);

	Vector3 span = sphere.center - ret;
	span.Normalize();
	span *= sphere.radius;
	span += ret;
	auto transform = GetGameObject()->GetComponent<Transform>();

	transform->SetWorldPosition(span);
}

void SphereCollision::CollisionEscape(CapsuleCollision& opponent)
{
}

void SphereCollision::CollisionEscape(RayCollision& opponent)
{
}
