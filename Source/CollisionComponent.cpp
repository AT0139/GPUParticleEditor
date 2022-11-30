#include "CollisionComponent.h"
#include "SphereCollision.h"
#include "Rigidbody.h"
#include "GameObject.h"

CollisionComponent::CollisionComponent(GameObject* pGameObject)
	: Component(pGameObject)
	, m_isStaticObject(false)
	, m_hitAction(HitAction::Auto)
{}

void CollisionComponent::HittingProcess(const CollisionComponent& DestColl, const D3DXVECTOR3& SrcVelocity, const D3DXVECTOR3& DestVelocity,
	D3DXVECTOR3 HitNormal, float AfterHitTime)
{
	auto PtrTransform = GetGameObject()->GetComponent<Transform>();
	//親の速度
	auto PtrDestTransform = DestColl.GetGameObject()->GetComponent<Transform>();
	auto DestVelo = PtrDestTransform->GetWorldPosition()
		- PtrDestTransform->GetPrevWorldPosition();

	bool horizontal = false;
	//if (GetGameObject()->FindBehavior<Gravity>()) {
	//	auto Grav = GetGameObject()->GetBehavior<Gravity>()->GetGravity();
	//	Grav.normalize();
	//	bsm::Vec3 v = (bsm::Vec3)XMVector3AngleBetweenNormals(Grav, HitNormal);
	//	if (v.x <= XM_PIDIV4) {
	//		horizontal = true;
	//	}
	//}

	//スライドする方向を計算
	D3DXVECTOR3 slide = Slide(SrcVelocity, HitNormal);
	PtrTransform->SetToPrev();
	if (GetHitAction() != HitAction::Stop && GetHitAction() != HitAction::None) {
		auto WorldPos = PtrTransform->GetWorldPosition() + slide * AfterHitTime;
		if (!horizontal) {
			WorldPos += HitNormal * (-1.0f * 0/*GetEscapeSlideValue()*/);
		}
		PtrTransform->SetWorldPosition(WorldPos);
	}


	auto PtrRigid = GetGameObject()->GetComponent<Rigidbody>();
	if (PtrRigid)
	{
		switch (GetHitAction())
		{
		case HitAction::Slide:
		{
			auto DestVelocity = PtrDestTransform->GetVelocity();
			slide = Slide(SrcVelocity, HitNormal);
			PtrRigid->SetVelocity(slide);
			//重力は0にする
			PtrRigid->SetGravityZero();
		}
		break;
		case HitAction::Auto:
			if (horizontal)
			{
				//乗っているときはスライドさせる
				slide = Slide(SrcVelocity, HitNormal);
				PtrRigid->SetVelocity(slide);
				//何かに乗ったときは重力は0にする
				PtrRigid->SetGravityZero();
			}
			else
			{
				//乗ってないときは反発
				auto DestRigid = DestColl.GetGameObject()->GetComponent<Rigidbody>();
				float ResultPower = -(1.0f + PtrRigid->GetBounciness());
				if (DestRigid) {
					D3DXVECTOR3 RelativeVelo = SrcVelocity - DestVelocity;
					ResultPower = (-(1.0f + PtrRigid->GetBounciness()) * D3DXVec3Dot(&RelativeVelo, &HitNormal)) /
						(D3DXVec3Dot(&HitNormal, &HitNormal) * (1 / PtrRigid->GetMass() + 1 / DestRigid->GetMass()));
				}
				else {
					D3DXVECTOR3 RelativeVelo = SrcVelocity;
					ResultPower = (-(1.0f + PtrRigid->GetBounciness()) * D3DXVec3Dot(&RelativeVelo, &HitNormal)) /
						(D3DXVec3Dot(&HitNormal, &HitNormal) * (1 / PtrRigid->GetMass()));
				}
				auto Velo = PtrRigid->GetVelocity();
				Velo += (HitNormal * ResultPower) / PtrRigid->GetMass();
				PtrRigid->SetVelocity(Velo);
			}
			break;
		case HitAction::Stop:
		{
			//速度は0にする
			PtrRigid->SetVelocityZero();
			//重力は0にする
			PtrRigid->SetGravityZero();
		}
		break;
		}
	}
}

D3DXVECTOR3 CollisionComponent::Slide(const D3DXVECTOR3& Vec, const D3DXVECTOR3& Norm)
{
	//thisと法線から直行線の長さ（内積で求める）
	float Len = D3DXVec3Dot(&Vec, &Norm);
	//その長さに伸ばす
	D3DXVECTOR3 Contact = Norm * Len;
	//スライドする方向は現在のベクトルから引き算
	return (Vec - Contact);
}

bool CollisionComponent::IsHitObject(GameObject* obj)
{
	if (!m_hitObjects.empty())
	{
		for (auto object : m_hitObjects)
		{
			if (object == obj)
			{
				return true;
			}
		}
	}
	return false;
}

void CollisionComponent::CollisionReset()
{
	m_hitObjects.clear();
}

