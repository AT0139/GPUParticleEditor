#include "OBBCollision.h"
#include "GameObject.h"
#include "CollisionUtility.h"
#include "DrawModel.h"
#include "Renderer.h"

OBBCollision::OBBCollision(GameObject* pGameObject)
	: CollisionComponent(pGameObject)
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
	if (CollisionUtility::ObbSphere(GetOBBInfo(), opponent.GetSphereInfo(),Vector3(0,0,0)))
	{
		AfterCollisionTemplate(this, &opponent);
	}
}

void OBBCollision::HitTest(AABBCollision& opponent)
{
	if (CollisionUtility::ObbAabb(GetOBBInfo(), opponent.GetAABBInfo()))
	{
		AfterCollisionTemplate(this, &opponent);
	}
}

void OBBCollision::HitTest(OBBCollision& opponent)
{
	if (CollisionUtility::ObbObb(GetOBBInfo(), opponent.GetOBBInfo()))
	{
		AfterCollisionTemplate(this, &opponent);
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
	obb.scaleHalf = GetCollisionScale();

	return obb;
}

void OBBCollision::SetScale(Vector3 scale)
{
	auto temp = scale * 0.5f;
	SetCollisionScale(temp);
}

void OBBCollision::SetScale(float scale)
{
	Vector3 sc = Vector3(scale, scale, scale);
	auto temp = sc * 0.5f;
	SetCollisionScale(temp);
}

Vector3 OBBCollision::GetHitNormal(SphereCollision& opponent)
{
	SphereInfo sp = opponent.GetSphereInfo();
	OBBInfo obb = GetOBBInfo();
	Vector3 normal;
	CollisionUtility::ObbSphere(obb, sp, normal);
	//接点へのベクトル
	normal = sp.center - normal;
	normal.Normalize();
	return normal;
}

Vector3 OBBCollision::GetHitNormal(AABBCollision& opponent)
{
	OBBInfo obb = GetOBBInfo();
	AABBInfo aabb = opponent.GetAABBInfo();
	//Rectのベクトル
	Vector3 ret = aabb.GetPlane(opponent.GetGameObject()).normal;
	ret.Normalize();
	return ret;
}

Vector3 OBBCollision::GetHitNormal(OBBCollision& opponent)
{
	OBBInfo obb = GetOBBInfo();
	OBBInfo obb2 = opponent.GetOBBInfo();
	Vector3 normal;
	//SrcのOBBとDestの最近接点を得る
	CollisionUtility::ClosestPtPointOBB(obb.center, obb2, normal);
	//接点へのベクトル
	Vector3 ret = normal - obb.center;
	ret.Normalize();
	return ret;
}

Vector3 OBBCollision::GetHitNormal(CapsuleCollision& opponent)
{
	return Vector3();
}

void OBBCollision::HitTest(CapsuleCollision& opponent)
{
}
