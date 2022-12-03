#include "CollisionUtility.h"

bool CollisionUtility::SphereSphere(SphereInfo Sphere1, SphereInfo Sphere2)
{

	D3DXVECTOR3 dir = Sphere1.center - Sphere2.center;

	float rad = Sphere1.radius + Sphere2.radius;
	float length = D3DXVec3Length(&dir);

	return length < rad;
}

bool CollisionUtility::AabbAabb(AABBInfo box1, AABBInfo box2)
{
	D3DXVECTOR3 box1min = box1.center - box1.scaleHalf;
	D3DXVECTOR3 box1max = box1.center + box1.scaleHalf;
	D3DXVECTOR3 box2min = box2.center - box2.scaleHalf;
	D3DXVECTOR3 box2max = box2.center + box2.scaleHalf;

	return (box1min.x <= box2max.x && box1max.x >= box2min.x) &&
		(box1min.y <= box2max.y && box1max.y >= box2min.y) &&
		(box1min.z <= box2max.z && box1max.z >= box2min.z);
}
