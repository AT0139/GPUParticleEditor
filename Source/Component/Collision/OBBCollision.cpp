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
	Vector3 temp(0, 0, 0);
	if (CollisionUtility::ObbSphere(GetOBBInfo(), opponent.GetSphereInfo(), temp))
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
	OBBInfo m_obb;
	auto transform = GetGameObject()->GetComponent<Transform>();
	m_obb.center = transform->GetPosition() + m_centerPosition;
	auto x = transform->GetXAxis();
	auto y = transform->GetYAxis();
	auto z = transform->GetZAxis();
	x.Normalize(); y.Normalize(); z.Normalize();
	m_obb.rot[0] = x;
	m_obb.rot[1] = y;
	m_obb.rot[2] = z;
	auto scale = GetCollisionScale();
	m_obb.scaleHalf[0] = scale.x;
	m_obb.scaleHalf[1] = scale.y;
	m_obb.scaleHalf[2] = scale.z;

	m_obb.orientation = Quaternion::Identity;
	m_obb.orientation = transform->GetRotation();
	m_obb.orientation.Normalize();

	return m_obb;
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
	auto m_obb = GetOBBInfo();
	auto sphere = opponent.GetSphereInfo();
	Vector3 ret;
	bool hit = CollisionUtility::ObbSphere(m_obb, sphere, ret);
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
		auto Pos = transform->GetWorldMatrix().Decompose(scale, rot, pos);
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
	auto transform = GetGameObject()->GetComponent<Transform>();

	auto myObb = GetOBBInfo();
	auto oppObb = opponent.GetOBBInfo();

	float myBase = myObb.center.y - myObb.scaleHalf[1];
	float oppHigh = oppObb.center.y + oppObb.scaleHalf[1];

	//微妙な段差は乗り越え
	if (oppHigh - myBase < 0.2f)
	{
		auto pos = transform->GetPosition();
		pos.y = oppHigh;
		transform->SetPosition(pos);
		return;
	}

	Vector3 ret;
	//最近接点を得る
	auto pos =GetGameObject()->GetComponent<Transform>()->GetPosition();
	CollisionUtility::ClosestPtPointOBB(pos, oppObb, ret);
	Vector3 span = pos - ret;
	span.Normalize();
	span *= 0.02f;
	auto center = pos;
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

	//エスケープはリセット
	transform->SetWorldPosition(center);
}

void OBBCollision::CollisionEscape(CapsuleCollision& opponent)
{
}

void OBBCollision::CollisionEscape(RayCollision& opponent)
{
}

Vector3 OBBCollision::GetHitNormal(SphereCollision& opponent)
{
	SphereInfo sp = opponent.GetSphereInfo();
	OBBInfo m_obb = GetOBBInfo();
	Vector3 normal;
	CollisionUtility::ObbSphere(m_obb, sp, normal);
	//接点へのベクトル
	normal = sp.center - normal;
	normal.Normalize();
	return normal;
}

Vector3 OBBCollision::GetHitNormal(AABBCollision& opponent)
{
	OBBInfo m_obb = GetOBBInfo();
	AABBInfo aabb = opponent.GetAABBInfo();
	//Rectのベクトル
	Vector3 ret = aabb.GetPlane(opponent.GetGameObject()).normal;
	ret.Normalize();
	return ret;
}

Vector3 OBBCollision::GetHitNormal(OBBCollision& opponent)
{
	OBBInfo m_obb = GetOBBInfo();
	OBBInfo obb2 = opponent.GetOBBInfo();
	Vector3 normal;
	//SrcのOBBとDestの最近接点を得る
	CollisionUtility::ClosestPtPointOBB(m_obb.center, obb2, normal);
	//接点へのベクトル
	Vector3 ret = normal - m_obb.center;
	ret.Normalize();
	return ret;
}

Vector3 OBBCollision::GetHitNormal(CapsuleCollision& opponent)
{
	return Vector3();
}

Vector3 OBBCollision::GetHitNormal(RayCollision& opponent)
{
	return Vector3();
}

void OBBCollision::HitTest(CapsuleCollision& opponent)
{
}


void OBBCollision::HitTest(RayCollision& opponent)
{
	
}
