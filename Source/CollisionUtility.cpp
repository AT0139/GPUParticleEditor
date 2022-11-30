#include "CollisionUtility.h"

bool CollisionUtility::SphereSphere(SphereInfo Sphere1, SphereInfo Sphere2)
{

	D3DXVECTOR3 dir = Sphere1.center - Sphere2.center;
	float dist = D3DXVec3Dot(&dir, &dir);
	float rad = Sphere1.radius + Sphere2.radius;

	return dist < rad* rad;
}

bool CollisionUtility::TestSphereSphere(const SphereInfo& SrcSp, const D3DXVECTOR3& SrcVelocity,
	const SphereInfo& DestSp, float StartTime, float EndTime, float& HitTime) {
	const float m_EPSILON = 0.005f;
	SphereInfo SrcSp2;
	float mid = (StartTime + EndTime) * 0.5f;
	SrcSp2.center = SrcSp.center + SrcVelocity * mid;
	SrcSp2.radius = (mid - StartTime) * D3DXVec3Length(&SrcVelocity) + SrcSp.radius;
	if (!CollisionUtility::SphereSphere(SrcSp2, DestSp)) {
		return false;
	}
	if (EndTime - StartTime < m_EPSILON) {
		HitTime = StartTime;
		return true;
	}
	if (TestSphereSphere(SrcSp, SrcVelocity, DestSp, StartTime, mid, HitTime)) {
		return true;
	}
	return TestSphereSphere(SrcSp, SrcVelocity, DestSp, mid, EndTime, HitTime);
}
