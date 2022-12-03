#pragma once

#include "CollisionComponent.h"

class CollisionUtility
{
public:
	static bool SphereSphere(SphereInfo sphere1, SphereInfo sphere2);
	static bool AabbAabb(AABBInfo box1, AABBInfo box2);
	static bool AABBSphere(AABBInfo box, SphereInfo sphere);
};