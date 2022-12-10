﻿#include "AABBCollision.h"
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
		//当たっている
		CollisonAfter(this, &opponent);
	}
}

void AABBCollision::HitTest(AABBCollision& opponent)
{
	auto myAabb = GetAABBInfo();
	auto oppAabb = opponent.GetAABBInfo();

	if (CollisionUtility::AabbAabb(myAabb, oppAabb))
	{
		//当たっている
		CollisonAfter(this, &opponent);
	}
}

void AABBCollision::HitTest(OBBCollision& opponent)
{
	if (CollisionUtility::ObbAabb(opponent.GetOBBInfo(), GetAABBInfo()))
	{
		CollisonAfter(this, &opponent);
	}
}

AABBInfo AABBCollision::GetAABBInfo()
{
	AABBInfo aabb;
	auto transform = GetGameObject()->GetComponent<Transform>();
	aabb.center = transform->GetPosition();
	aabb.scaleHalf = GetCollisionScale();
	return aabb;
}

void AABBCollision::HitTest(CapsuleCollision& opponent)
{
}
