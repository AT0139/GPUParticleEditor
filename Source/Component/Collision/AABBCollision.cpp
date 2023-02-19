#include "AABBCollision.h"
#include "GameObject.h"
#include "CollisionUtility.h"
#include "DrawModel.h"
#include "Renderer.h"

AABBCollision::AABBCollision(GameObject* pGameObject)
	: CollisionComponent(pGameObject)
{
#ifdef _DEBUG
	//todo: AABB可視化を軸平行に(回転しないように)
	m_model = GetGameObject()->AddComponent<DrawModel>(GetGameObject());
	m_model->Load("asset\\model\\Cube.obj");
	m_model->SetRasterizerState(RASTERIZER::WIRE_FRAME);
	m_model->SetCollisionDraw(true);
#endif // _DEBUG
}

AABBCollision::~AABBCollision()
{
}

void AABBCollision::Update()
{
}

void AABBCollision::Draw()
{
}

void AABBCollision::SetScale(Vector3 scale)
{
	auto sc = scale * 0.5f;
#ifdef _DEBUG
	SetCollisionScale(sc);
#endif // _DEBUG
}

void AABBCollision::CollisionBridge(const std::shared_ptr<CollisionComponent>& opponent)
{
	//ダブルディスパッチ
	opponent->HitTest(*this);
}

void AABBCollision::HitTest(SphereCollision& opponent)
{
	auto oppSphere = opponent.GetSphereInfo();
	auto myAABB = GetAABBInfo();

	if (CollisionUtility::AABBSphere(myAABB, oppSphere))
	{
		AfterCollisionTemplate(this, &opponent);
	}
}

void AABBCollision::HitTest(AABBCollision& opponent)
{
	auto myAabb = GetAABBInfo();
	auto oppAabb = opponent.GetAABBInfo();

	if (CollisionUtility::AabbAabb(myAabb, oppAabb))
	{
		AfterCollisionTemplate(this, &opponent);
	}
}

void AABBCollision::HitTest(OBBCollision& opponent)
{
	if (CollisionUtility::ObbAabb(opponent.GetOBBInfo(), GetAABBInfo()))
	{
		AfterCollisionTemplate(this, &opponent);
	}
}

AABBInfo AABBCollision::GetAABBInfo()
{
	AABBInfo aabb;
	auto transform = GetGameObject()->GetComponent<Transform>();
	aabb.center = transform->GetPosition() + m_centerPosition;
	aabb.scaleHalf = GetCollisionScale();
	return aabb;
}

void AABBCollision::CollisionEscape(SphereCollision& opponent)
{
}

void AABBCollision::CollisionEscape(AABBCollision& opponent)
{
}

void AABBCollision::CollisionEscape(OBBCollision& opponent)
{
}

void AABBCollision::CollisionEscape(CapsuleCollision& opponent)
{
}

void AABBCollision::CollisionEscape(RayCollision& opponent)
{
}

Vector3 AABBCollision::GetHitNormal(SphereCollision& opponent)
{
	return Vector3();
}

Vector3 AABBCollision::GetHitNormal(AABBCollision& opponent)
{
	return Vector3();
}

Vector3 AABBCollision::GetHitNormal(OBBCollision& opponent)
{
	return Vector3();
}

Vector3 AABBCollision::GetHitNormal(CapsuleCollision& opponent)
{
	return Vector3();
}

Vector3 AABBCollision::GetHitNormal(RayCollision& opponent)
{
	return Vector3();
}

void AABBCollision::HitTest(CapsuleCollision& opponent)
{
}

void AABBCollision::HitTest(RayCollision& opponent)
{
}
