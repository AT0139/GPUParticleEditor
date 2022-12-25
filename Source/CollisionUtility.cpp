#include "CollisionUtility.h"

bool CollisionUtility::SphereSphere(SphereInfo sphere1, SphereInfo sphere2)
{
	Vector3 dir = sphere1.center - sphere2.center;

	float rad = sphere1.radius + sphere2.radius;
	float length = Utility::VECtoFloat(XMVector3LengthEst(dir));

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

bool CollisionUtility::ObbObb(OBBInfo box1, OBBInfo box2)
{
	const float EPSILON = 1.175494e-37f;
	float R[3][3], AbsR[3][3];
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			R[i][j] = box1.rot[i].Dot(box2.rot[j]);
			AbsR[i][j] = fabsf(R[i][j]) + EPSILON;
		}
	}
	Vector3 temp = box2.center - box1.center;
	float t[3];
	t[0] = temp.Dot(box1.rot[0]);
	t[1] = temp.Dot(box1.rot[1]);
	t[2] = temp.Dot(box1.rot[2]);
	//軸L=A0, L=A1, L=A2判定
	float ra, rb;
	for (int i = 0; i < 3; i++)
	{
		ra = box1.scaleHalf[i];
		rb = box2.scaleHalf[0] * AbsR[i][0] + box2.scaleHalf[1] * AbsR[i][1] + box2.scaleHalf[2] * AbsR[i][2];
		if (fabsf(t[i]) > ra + rb)
		{
			return false;
		}
	}
	//軸L=B0, L=B1, L=B2判定
	for (int i = 0; i < 3; i++)
	{
		ra = box1.scaleHalf[0] * AbsR[0][i] + box1.scaleHalf[1] * AbsR[1][i] + box1.scaleHalf[2] * AbsR[2][i];
		rb = box2.scaleHalf[i];
		if (fabsf(t[0] * R[0][i] + t[1] * R[1][i] + t[2] * R[2][i]) > ra + rb)
		{
			return false;
		}
	}
	//軸L=A0 X B0判定
	ra = box1.scaleHalf[1] * AbsR[2][0] + box1.scaleHalf[2] * AbsR[1][0];
	rb = box2.scaleHalf[1] * AbsR[0][2] + box2.scaleHalf[2] * AbsR[0][1];
	if (fabsf(t[2] * R[1][0] - t[1] * R[2][0]) > ra + rb)
	{
		return false;
	}
	//軸L=A0 X B1判定
	ra = box1.scaleHalf[1] * AbsR[2][1] + box1.scaleHalf[2] * AbsR[1][1];
	rb = box2.scaleHalf[0] * AbsR[0][2] + box2.scaleHalf[2] * AbsR[0][0];
	if (fabsf(t[2] * R[1][1] - t[1] * R[2][1]) > ra + rb)
	{
		return false;
	}
	//軸L=A0 X B2判定
	ra = box1.scaleHalf[1] * AbsR[2][2] + box1.scaleHalf[2] * AbsR[1][2];
	rb = box2.scaleHalf[0] * AbsR[0][1] + box2.scaleHalf[1] * AbsR[0][0];
	if (fabsf(t[2] * R[1][2] - t[1] * R[2][2]) > ra + rb)
	{
		return false;
	}
	//軸L=A1 X B0判定
	ra = box1.scaleHalf[0] * AbsR[2][0] + box1.scaleHalf[2] * AbsR[0][0];
	rb = box2.scaleHalf[1] * AbsR[1][2] + box2.scaleHalf[2] * AbsR[1][1];
	if (fabsf(t[0] * R[2][0] - t[2] * R[0][0]) > ra + rb)
	{
		return false;
	}
	//軸L=A1 X B1判定
	ra = box1.scaleHalf[0] * AbsR[2][1] + box1.scaleHalf[2] * AbsR[0][1];
	rb = box2.scaleHalf[0] * AbsR[1][2] + box2.scaleHalf[2] * AbsR[1][0];
	if (fabsf(t[0] * R[2][1] - t[2] * R[0][1]) > ra + rb)
	{
		return false;
	}
	//軸L=A1 X B2判定
	ra = box1.scaleHalf[0] * AbsR[2][2] + box1.scaleHalf[2] * AbsR[0][2];
	rb = box2.scaleHalf[0] * AbsR[1][1] + box2.scaleHalf[1] * AbsR[1][0];
	if (fabsf(t[0] * R[2][2] - t[2] * R[0][2]) > ra + rb)
	{
		return false;
	}
	//軸L=A2 X B0判定
	ra = box1.scaleHalf[0] * AbsR[1][0] + box1.scaleHalf[1] * AbsR[0][0];
	rb = box2.scaleHalf[1] * AbsR[2][2] + box2.scaleHalf[2] * AbsR[2][1];
	if (fabsf(t[1] * R[0][0] - t[0] * R[1][0]) > ra + rb)
	{
		return false;
	}
	//軸L=A2 X B1判定
	ra = box1.scaleHalf[0] * AbsR[1][1] + box1.scaleHalf[1] * AbsR[0][1];
	rb = box2.scaleHalf[0] * AbsR[2][2] + box2.scaleHalf[2] * AbsR[2][0];
	if (fabsf(t[1] * R[0][1] - t[0] * R[1][1]) > ra + rb)
	{
		return false;
	}
	//軸L=A2 X B2判定
	ra = box1.scaleHalf[0] * AbsR[1][2] + box1.scaleHalf[1] * AbsR[0][2];
	rb = box2.scaleHalf[0] * AbsR[2][1] + box2.scaleHalf[1] * AbsR[2][0];
	if (fabsf(t[1] * R[0][2] - t[0] * R[1][2]) > ra + rb)
	{
		return false;
	}
	return true;
}

bool CollisionUtility::ObbSphere(OBBInfo m_obb, SphereInfo sphere, Vector3& returnNormal)
{
	ClosestPtPointOBB(sphere.center, m_obb, returnNormal);
	Vector3 v = returnNormal - sphere.center;
	return v.Dot(v) <= sphere.radius * sphere.radius;
}

bool CollisionUtility::ObbAabb(OBBInfo m_obb, AABBInfo aabb)
{
	OBBInfo obb2;
	obb2.center = aabb.center;
	obb2.scaleHalf[0] = aabb.scaleHalf.x;
	obb2.scaleHalf[1] = aabb.scaleHalf.y;
	obb2.scaleHalf[2] = aabb.scaleHalf.z;
	obb2.rot[0] = Vector3(1, 0, 0);
	obb2.rot[1] = Vector3(0, 1, 0);
	obb2.rot[2] = Vector3(0, 0, 1);

	return 	ObbObb(m_obb, obb2);
}

void CollisionUtility::ClosestPtPointOBB(Vector3 point, OBBInfo m_obb, Vector3& normal)
{
	Vector3 d = point - m_obb.center;
	normal = m_obb.center;
	float dist;
	for (int i = 0; i < 3; i++)
	{
		dist = d.Dot(m_obb.rot[i]);
		if (dist > m_obb.scaleHalf[i])
		{
			dist = m_obb.scaleHalf[i];
		}
		if (dist < -m_obb.scaleHalf[i])
		{
			dist = -m_obb.scaleHalf[i];
		}
		normal += dist * m_obb.rot[i];
	}
}

bool CollisionUtility::RayObb(RayInfo ray, OBBInfo obb)
{
	BoundingOrientedBox dxObb;
	dxObb.Center = obb.center;
	dxObb.Extents.x = obb.scaleHalf[0];
	dxObb.Extents.y = obb.scaleHalf[1];
	dxObb.Extents.z = obb.scaleHalf[2];
	dxObb.Orientation = obb.orientation;

	float len;
	if (dxObb.Intersects(ray.position, ray.direction, len))
	{
		if (len < ray.length)
			return true;
	}

	return false;
}

bool CollisionUtility::RaySphere(RayInfo ray, SphereInfo sphere)
{
	BoundingSphere sp;
	sp.Center = sphere.center;
	sp.Radius = sphere.radius;

	float len;
	if(sp.Intersects(ray.position, ray.direction, len))
	{
		if (len < ray.length)
			return true;
	}

	return false;
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
	return Utility::VECtoFloat(XMVector3LengthEst(dir)) < sphere.radius;
}