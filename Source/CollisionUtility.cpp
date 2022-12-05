﻿#include "CollisionUtility.h"

bool CollisionUtility::SphereSphere(SphereInfo sphere1, SphereInfo sphere2)
{

	Vector3 dir = sphere1.center - sphere2.center;

	float rad = sphere1.radius + sphere2.radius;
	float length = D3DXVec3Length(&dir);

	return length < rad;
}

bool CollisionUtility::AabbAabb(AABBInfo box1, AABBInfo box2)
{
	Vector3 box1min = box1.center - box1.scaleHalf;
	Vector3 box1max = box1.center + box1.scaleHalf;
	Vector3 box2min = box2.center - box2.scaleHalf;
	Vector3 box2max = box2.center + box2.scaleHalf;

	return (box1min.x <= box2max.x && box1max.x >= box2min.x) &&
		(box1min.y <= box2max.y && box1max.y >= box2min.y) &&
		(box1min.z <= box2max.z && box1max.z >= box2min.z);
}

bool getSeparatingPlane(const Vector3& RPos, const Vector3& Plane, const OBBInfo& box1, const OBBInfo& box2)
{
	auto x1 = box1.X * box1.scaleHalf.x;
	auto y1 = box1.Y * box1.scaleHalf.y;
	auto z1 = box1.Z * box1.scaleHalf.z;
	auto x2 = box2.X * box2.scaleHalf.x;
	auto y2 = box2.Y * box2.scaleHalf.y;
	auto z2 = box2.Z * box2.scaleHalf.z;

	return (fabs(D3DXVec3Dot(&RPos, &Plane)) >
			(fabs(D3DXVec3Dot(&x1, &Plane)) +
			fabs(D3DXVec3Dot(&y1, &Plane)) +
			fabs(D3DXVec3Dot(&z1, &Plane)) +
			fabs(D3DXVec3Dot(&x2, &Plane)) +
			fabs(D3DXVec3Dot(&y2, &Plane)) +
			fabs(D3DXVec3Dot(&z2, &Plane))));
}

bool CollisionUtility::ObbObb(OBBInfo box1, OBBInfo box2)
{
	Vector3 direction;
	Vector3 temp;
	direction = box2.center - box1.center;
	//軸ごとに比較
	return !(getSeparatingPlane(direction, box1.X, box1, box2) ||
		getSeparatingPlane(direction, box1.Y, box1, box2) ||
		getSeparatingPlane(direction, box1.Z, box1, box2) ||
		getSeparatingPlane(direction, box2.X, box1, box2) ||
		getSeparatingPlane(direction, box2.Y, box1, box2) ||
		getSeparatingPlane(direction, box2.Z, box1, box2) ||
		getSeparatingPlane(direction, *D3DXVec3Cross(&temp, &box1.X, &box2.X), box1, box2) ||
		getSeparatingPlane(direction, *D3DXVec3Cross(&temp, &box1.X, &box2.Y), box1, box2) ||
		getSeparatingPlane(direction, *D3DXVec3Cross(&temp, &box1.X, &box2.Z), box1, box2) ||
		getSeparatingPlane(direction, *D3DXVec3Cross(&temp, &box1.Y, &box2.X), box1, box2) ||
		getSeparatingPlane(direction, *D3DXVec3Cross(&temp, &box1.Y, &box2.Y), box1, box2) ||
		getSeparatingPlane(direction, *D3DXVec3Cross(&temp, &box1.Y, &box2.Z), box1, box2) ||
		getSeparatingPlane(direction, *D3DXVec3Cross(&temp, &box1.Z, &box2.X), box1, box2) ||
		getSeparatingPlane(direction, *D3DXVec3Cross(&temp, &box1.Z, &box2.Y), box1, box2) ||
		getSeparatingPlane(direction, *D3DXVec3Cross(&temp, &box1.Z, &box2.Z), box1, box2));
}

bool CollisionUtility::ObbSphere(OBBInfo obb, SphereInfo sphere)
{
	Vector3 direction = sphere.center - obb.center;

	float obbLenX, obbLenZ;
	obbLenX = D3DXVec3Length(&obb.X);
	obb.X /= obbLenX;

	obbLenZ = D3DXVec3Length(&obb.Z);
	obb.Z /= obbLenZ;

	float lenX, lenZ;
	lenX = D3DXVec3Dot(&obb.X, &direction);
	lenZ = D3DXVec3Dot(&obb.Z, &direction);

	return (fabs(lenX) < obbLenX + sphere.radius && fabs(lenZ) < obbLenZ + sphere.radius);
}

bool CollisionUtility::ObbAabb(OBBInfo obb, AABBInfo aabb)
{
	OBBInfo obb2;
	obb2.center = aabb.center;
	obb2.scaleHalf = aabb.scaleHalf;
	obb2.X = Vector3(1, 0, 0);
	obb2.Y = Vector3(0, 1, 0);
	obb2.Z = Vector3(0, 0, 1);

	return 	ObbObb(obb, obb2);
}

bool CollisionUtility::AABBSphere(AABBInfo box, SphereInfo sphere)
{
	Vector3 boxMin = box.center - box.scaleHalf;
	Vector3 boxMax = box.center + box.scaleHalf;
	Vector3 nearPoint;
	nearPoint.x = std::max(boxMin.x, std::min(sphere.center.x, boxMax.x));
	nearPoint.y = std::max(boxMin.y, std::min(sphere.center.y, boxMax.y));
	nearPoint.z = std::max(boxMin.z, std::min(sphere.center.z, boxMax.z));

	auto dir = nearPoint - sphere.center;
	return D3DXVec3Length(&dir) < sphere.radius;
}
