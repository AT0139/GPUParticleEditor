#include "CollisionUtility.h"

bool CollisionUtility::SphereSphere(SphereInfo Sphere1, SphereInfo Sphere2)
{

	D3DXVECTOR3 dir = Sphere1.center - Sphere2.center;

	float rad = Sphere1.radius + Sphere2.radius;
	float length = D3DXVec3Length(&dir);

	return length < rad;
}