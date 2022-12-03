#pragma once

#include "CollisionComponent.h"

class CollisionUtility
{
public:
	static bool SphereSphere(SphereInfo Sphere1, SphereInfo Sphere2);
	static bool AabbAabb(AABBInfo box1, AABBInfo box2);
};