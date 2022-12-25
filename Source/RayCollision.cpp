#include "RayCollision.h"
#include "GameObject.h"
#include "CollisionUtility.h"
#include "DrawModel.h"
#include "Renderer.h"

RayCollision::RayCollision(GameObject* pGameObject)
	: CollisionComponent(pGameObject)
{
#ifdef _DEBUG
	m_model = GetGameObject()->AddComponent<DrawModel>(GetGameObject());
	m_model->Load("asset\\model\\Cube.obj");
	m_model->SetRasterizerState(RASTERIZER::WIRE_FRAME);
	m_model->SetCollisionDraw(true);
#endif // _DEBUG
}

RayCollision::~RayCollision()
{
}

void RayCollision::Update()
{
}

void RayCollision::Draw()
{
}

void RayCollision::CollisionBridge(const std::shared_ptr<CollisionComponent>& opponent)
{
	//ダブルディスパッチ
	opponent->HitTest(*this);
}

void RayCollision::HitTest(SphereCollision& opponent)
{
	if (CollisionUtility::RaySphere(GetRayInfo(), opponent.GetSphereInfo()))
	{
		AfterCollisionTemplate(this, &opponent);
	}
}

void RayCollision::HitTest(AABBCollision& opponent)
{
}

void RayCollision::HitTest(OBBCollision& opponent)
{
	if (CollisionUtility::RayObb(GetRayInfo(), opponent.GetOBBInfo()))
	{
		AfterCollisionTemplate(this, &opponent);
	}
}

void RayCollision::CollisionEscape(SphereCollision& opponent)
{
}

void RayCollision::CollisionEscape(AABBCollision& opponent)
{
}

void RayCollision::CollisionEscape(OBBCollision& opponent)
{
}

void RayCollision::CollisionEscape(CapsuleCollision& opponent)
{
}

RayInfo RayCollision::GetRayInfo()
{
	RayInfo ray = m_info;
	auto transform = GetGameObject()->GetComponent<Transform>();
	Vector3::Transform(ray.direction, transform->GetRotation(), ray.direction);

	return ray;
}

void RayCollision::SetDirection(Vector3 dir)
{
	m_info.direction = dir;
	m_info.direction.Normalize();

	SetCollisionScale(m_info.direction * m_info.direction);
}

void RayCollision::SetLength(float len)
{
	m_info.length = len;

	SetCollisionScale(m_info.direction * m_info.direction);
}

void RayCollision::HitTest(RayCollision& opponent)
{
}

Vector3 RayCollision::GetHitNormal(RayCollision& opponent)
{
	return Vector3();
}

void RayCollision::CollisionEscape(RayCollision& opponent)
{
}

Vector3 RayCollision::GetHitNormal(SphereCollision& opponent)
{
	return Vector3();
}

Vector3 RayCollision::GetHitNormal(AABBCollision& opponent)
{
	return Vector3();
}

Vector3 RayCollision::GetHitNormal(OBBCollision& opponent)
{
	return Vector3();
}

Vector3 RayCollision::GetHitNormal(CapsuleCollision& opponent)
{
	return Vector3();
}

void RayCollision::HitTest(CapsuleCollision& opponent)
{
}
