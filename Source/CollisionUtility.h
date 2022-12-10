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
	static bool ObbAabb(OBBInfo obb, AABBInfo aabb);

	static bool CapsuleCapsule(CapsuleInfo cap1, CapsuleInfo cap2);

private:
	static float CalcSegmentSegmentDist(const LineInfo& s1, const LineInfo& s2, Vector3& p1, Vector3& p2, float& t1, float& t2);
	static float CalcPointLineDistance(const Vector3& point,const LineInfo& line, Vector3& hit, float t);
	static float CalcPointSegmentDist(const Vector3& point, const LineInfo& line, Vector3& h, float& t);
	static float CalcLineLineDistance(const LineInfo& line1, const LineInfo& line2, Vector3& point1, Vector3& point2, float& t1, float& t2);
	static bool IsSharpAngle(const Vector3& point1, const Vector3 point2);
	static bool IsSharpAngle3(const Vector3& point1, const Vector3 point2, const Vector3 point3);
	static bool IsParallel(const Vector3& v1, const Vector3& v2);
	static void Clamp01(float& v);
};