#include "CollisionUtility.h"

bool CollisionUtility::SphereSphere(SphereInfo sphere1, SphereInfo sphere2)
{

	D3DXVECTOR3 dir = sphere1.center - sphere2.center;

	float rad = sphere1.radius + sphere2.radius;
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

bool CollisionUtility::AABBSphere(AABBInfo box, SphereInfo sphere)
{
	D3DXVECTOR3 boxMin = box.center - box.scaleHalf;
	D3DXVECTOR3 boxMax = box.center + box.scaleHalf;
	D3DXVECTOR3 nearPoint;
	nearPoint.x = std::max(boxMin.x, std::min(sphere.center.x, boxMax.x));
	nearPoint.y = std::max(boxMin.y, std::min(sphere.center.y, boxMax.y));
	nearPoint.z = std::max(boxMin.z, std::min(sphere.center.z, boxMax.z));

	auto dir = nearPoint - sphere.center;
	return D3DXVec3Length(&dir) < sphere.radius;
}
