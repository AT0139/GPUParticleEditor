#include "OBBCollision.h"
#include "GameObject.h"
#include "CollisionUtility.h"
#include "DrawModel.h"
#include "Renderer.h"

OBBCollision::OBBCollision(GameObject* pGameObject)
	: CollisionComponent(pGameObject)
	, m_collisionScale(1.0f, 1.0f, 1.0)
{
#ifdef _DEBUG
	m_model = GetGameObject()->AddComponent<DrawModel>(GetGameObject());
	m_model->Load("asset\\model\\Cube.obj");
	m_model->SetRasterizerState(RASTERIZER::WIRE_FRAME);
	m_model->SetCollisionDraw(true);
#endif // _DEBUG
}

OBBCollision::~OBBCollision()
{
}

void OBBCollision::Update()
{
}

void OBBCollision::Draw()
{
}

void OBBCollision::CollisionBridge(const std::shared_ptr<CollisionComponent>& opponent)
{
	//ダブルディスパッチ
	opponent->HitTest(*this);
}

void OBBCollision::HitTest(SphereCollision& opponent)
{
	if (CollisionUtility::ObbSphere(GetOBBInfo(), opponent.GetSphereInfo()))
	{
		CollisonAfter(this, &opponent);
	}
}

void OBBCollision::HitTest(AABBCollision& opponent)
{
	if (CollisionUtility::ObbAabb(GetOBBInfo(), opponent.GetAABBInfo()))
	{
		CollisonAfter(this, &opponent);
	}
}

void OBBCollision::HitTest(OBBCollision& opponent)
{
	if (CollisionUtility::ObbObb(GetOBBInfo(), opponent.GetOBBInfo()))
	{
		CollisonAfter(this, &opponent);
	}
}

OBBInfo OBBCollision::GetOBBInfo()
{
	OBBInfo obb;
	auto transform = GetGameObject()->GetComponent<Transform>();
	obb.center = transform->GetPosition();
	obb.X = transform->GetXAxis();
	obb.Y = transform->GetYAxis();
	obb.Z = transform->GetZAxis();
	obb.scaleHalf = m_collisionScale;

	return obb;
}

void OBBCollision::SetCollisionScale(Vector3 scale)
{
	m_collisionScale = scale * 0.5f;
#ifdef _DEBUG
	GetGameObject()->GetComponent<Transform>()->SetCollisionScale(m_collisionScale);
#endif // _DEBUG
}

void OBBCollision::SetCollisionScale(float scale)
{
	Vector3 sc = Vector3(scale, scale, scale);
	m_collisionScale = sc * 0.5f;
#ifdef _DEBUG
	GetGameObject()->GetComponent<Transform>()->SetCollisionScale(m_collisionScale);
#endif // _DEBUG
}