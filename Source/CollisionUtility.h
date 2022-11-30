#pragma once

#include "CollisionComponent.h"

class CollisionUtility
{
public:
	static bool TestSphereSphere(const SphereInfo& SrcSp, const D3DXVECTOR3& SrcVelocity,
		const SphereInfo& DestSp, float StartTime, float EndTime, float& HitTime);
	static bool SphereSphere(SphereInfo Sphere1, SphereInfo Sphere2);
};