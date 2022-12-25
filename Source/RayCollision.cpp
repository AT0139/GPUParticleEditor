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
	return m_info;
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
