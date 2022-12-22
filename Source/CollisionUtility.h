#pragma once

#include "CollisionComponent.h"

class CollisionUtility
{
public:
	static bool SphereSphere(SphereInfo sphere1, SphereInfo sphere2);

	static bool AabbAabb(AABBInfo box1, AABBInfo box2);
	static bool AABBSphere(AABBInfo box, SphereInfo sphere);

	static bool ObbObb(OBBInfo box1, OBBInfo box2);
	static bool ObbSphere(OBBInfo m_obb, SphereInfo sphere, Vector3& returnNormal);
	static bool ObbAabb(OBBInfo m_obb, AABBInfo aabb);
	static void ClosestPtPointOBB(Vector3 point, OBBInfo m_obb, Vector3& normal);

	//static bool CapsuleCapsule(CapsuleInfo cap1, CapsuleInfo cap2);
private:
};