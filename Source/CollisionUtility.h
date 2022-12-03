#pragma once

#include "CollisionComponent.h"

class CollisionUtility
{
public:
	static bool SphereSphere(SphereInfo sphere1, SphereInfo sphere2);

	static bool AabbAabb(AABBInfo box1, AABBInfo box2);
	static bool AABBSphere(AABBInfo box, SphereInfo sphere);

	static bool ObbObb(OBBInfo box1, OBBInfo box2);
	static bool ObbSphere(OBBInfo obb, SphereInfo sphere);

private:
};