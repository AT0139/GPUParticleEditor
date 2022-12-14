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

void OBBCollision::CollisionEscape(SphereCollision& opponent)
{
	auto obb = GetOBBInfo();
	auto sphere = opponent.GetSphereInfo();
	Vector3 ret;
	bool hit = CollisionUtility::ObbSphere(obb, sphere, ret);
	if (hit)
	{
		Vector3 nowSpan = ret - sphere.center;
		Vector3 newSpan = nowSpan;
		newSpan.Normalize();
		newSpan *= (sphere.radius);
		auto MoveSpan = newSpan - nowSpan;
		auto transform = GetGameObject()->GetComponent<Transform>();
		Vector3 pos, scale;
		Quaternion rot;
		auto Pos = transform->GetWorldMatrix().Decompose(scale,rot,pos);
		pos += MoveSpan;
		//エスケープはリセット
		transform->SetWorldPosition(pos);
	}
}

void OBBCollision::CollisionEscape(AABBCollision& opponent)
{
}

void OBBCollision::CollisionEscape(OBBCollision& opponent)
{
	auto myObb = GetOBBInfo();
	auto oppObb = opponent.GetOBBInfo();

	Vector3 ret;
	//SrcのOBBとDestの最近接点を得る
	CollisionUtility::ClosestPtPointOBB(myObb.center, oppObb, ret);
	Vector3 span = myObb.center - ret;
	span.Normalize();
	span *= 0.02f;
	auto center = myObb.center;
	int count = 0;
	while (1)
	{
		center += span;
		myObb.center = center;
		if (!CollisionUtility::ObbObb(myObb, oppObb))
		{
			break;
		}
		count++;
		if (count > 50)
		{
			break;
		}
	}
	auto transform = GetGameObject()->GetComponent<Transform>();
	//エスケープはリセット
	transform->SetWorldPosition(center);
}

void OBBCollision::CollisionEscape(CapsuleCollision& opponent)
{

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
