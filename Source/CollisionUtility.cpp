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

bool getSeparatingPlane(const Vector3& RPos, const XMVECTOR& Plane, const OBBInfo& box1, const OBBInfo& box2)
{
	auto x1 = box1.X * box1.scaleHalf.x;
	auto y1 = box1.Y * box1.scaleHalf.y;
	auto z1 = box1.Z * box1.scaleHalf.z;
	auto x2 = box2.X * box2.scaleHalf.x;
	auto y2 = box2.Y * box2.scaleHalf.y;
	auto z2 = box2.Z * box2.scaleHalf.z;

	//todo : 正常なら消す
	return (fabs(Utility::VECtoFloat(XMVector3Dot(RPos, Plane)))) >
		(fabs(Utility::VECtoFloat(XMVector3Dot(x1, Plane))) +
			fabs(Utility::VECtoFloat(XMVector3Dot(y1, Plane))) +
			fabs(Utility::VECtoFloat(XMVector3Dot(z1, Plane))) +
			fabs(Utility::VECtoFloat(XMVector3Dot(x2, Plane))) +
			fabs(Utility::VECtoFloat(XMVector3Dot(y2, Plane))) +
			fabs(Utility::VECtoFloat(XMVector3Dot(z2, Plane))));
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
		getSeparatingPlane(direction, XMVector3Cross(box1.X, box2.X), box1, box2) ||
		getSeparatingPlane(direction, XMVector3Cross(box1.X, box2.Y), box1, box2) ||
		getSeparatingPlane(direction, XMVector3Cross(box1.X, box2.Z), box1, box2) ||
		getSeparatingPlane(direction, XMVector3Cross(box1.Y, box2.X), box1, box2) ||
		getSeparatingPlane(direction, XMVector3Cross(box1.Y, box2.Y), box1, box2) ||
		getSeparatingPlane(direction, XMVector3Cross(box1.Y, box2.Z), box1, box2) ||
		getSeparatingPlane(direction, XMVector3Cross(box1.Z, box2.X), box1, box2) ||
		getSeparatingPlane(direction, XMVector3Cross(box1.Z, box2.Y), box1, box2) ||
		getSeparatingPlane(direction, XMVector3Cross(box1.Z, box2.Z), box1, box2));
}

bool CollisionUtility::ObbSphere(OBBInfo obb, SphereInfo sphere, Vector3 returnNormal)
{
	ClosestPtPointOBB(sphere.center, obb, returnNormal);
	Vector3 v = returnNormal - sphere.center;
	return v.Dot(v) <= sphere.radius * sphere.radius;
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

void CollisionUtility::ClosestPtPointOBB(Vector3 point, OBBInfo obb, Vector3& normal)
{
	Vector3 dir = point - obb.center;
	normal = obb.center;
	float dist;
	{
		dist = dir.Dot(obb.X);
		if (dist > obb.scaleHalf.x)
		{
			dist = obb.scaleHalf.x;
		}
		if (dist < -obb.scaleHalf.x)
		{
			dist = -obb.scaleHalf.x;
		}
		normal += dist * obb.X;

		dist = dir.Dot(obb.Y);
		if (dist > obb.scaleHalf.y)
		{
			dist = obb.scaleHalf.y;
		}
		if (dist < -obb.scaleHalf.y)
		{
			dist = -obb.scaleHalf.y;
		}
		normal += dist * obb.Y;

		dist = dir.Dot(obb.Z);
		if (dist > obb.scaleHalf.z)
		{
			dist = obb.scaleHalf.z;
		}
		if (dist < -obb.scaleHalf.z)
		{
			dist = -obb.scaleHalf.z;
		}
		normal += dist * obb.Z;
	}
}

//bool CollisionUtility::CapsuleCapsule(CapsuleInfo cap1, CapsuleInfo cap2)
//{
//	Vector3 p1, p2;
//	float t1, t2;
//	float d = CalcSegmentSegmentDist(cap1.line, cap2.line, p1, p2, t1, t2);
//
//	return (d <= cap1.radius + cap2.radius);
//}
//
//float CollisionUtility::CalcSegmentSegmentDist(const LineInfo& s1, const LineInfo& s2, Vector3& p1, Vector3& p2, float& t1, float& t2)
//{
//	if (Utility::VECtoFloat(XMVector3LengthSq(s1.end)) < 0.00001f)
//	{
//		if (Utility::VECtoFloat(XMVector3LengthSq(s2.end)) < 0.00001f)
//		{
//			float len = Utility::VECtoFloat(XMVector3Length(s2.start - s1.start));
//			p1 = s1.start;
//			p2 = s2.start;
//			t1 = t2 = 0.0f;
//			return len;
//		}
//		else
//		{
//			float len = CalcPointSegmentDist(s1.start, s2, p2, t2);
//			p1 = s1.start;
//			t1 = 0.0f;
//			Clamp01(t2);
//			return len;
//		}
//	}
//	else if (Utility::VECtoFloat(XMVector3LengthSq(s2.end)) < 0.00001f)
//	{
//		float len = CalcPointSegmentDist(s2.start, s1, p1, t1);
//		p2 = s2.start;
//		Clamp01(t1);
//		t2 = 0.0f;
//		return len;
//	}
//
//	if (IsParallel(s1.end, s2.end))
//	{
//		t1 = 0.0f;
//		p1 = s1.start;
//		float len = CalcPointSegmentDist(s1.start, s2, p2, t2);
//		if (0.0f <= t2 && t2 <= 1.0f)
//		{
//			return len;
//		}
//	}
//	else
//	{
//		float len = CalcLineLineDistance(s1, s2, p1, p2, t1, t2);
//		if (0.0f <= t1 && t1 <= 1.0f && 0.0f <= t2 && t2 <= 1.0f)
//		{
//			return len;
//		}
//	}
//
//	Clamp01(t1);
//	p1 = s1.start + t1 * s1.end;
//	float len = CalcPointSegmentDist(p1, s2, p2, t2);
//	if (0.0f <= t2, t2 <= 1.0f)
//	{
//		return len;
//	}
//
//	Clamp01(t2);
//	p2 = s2.start + t2 * s2.end;
//	len = CalcPointSegmentDist(p2, s2, p1, t1);
//	if (0.0f <= t1 && t1 < 1.0f)
//	{
//		return len;
//	}
//
//	Clamp01(t1);
//	p1 = s1.start + t1 * s1.end;
//
//	return Utility::VECtoFloat(XMVector3Length(p2 - p1));
//}
//
//float CollisionUtility::CalcPointLineDistance(const Vector3& point,const LineInfo& line, Vector3& hit, float t)
//{
//	float lenSq = Utility::VECtoFloat(XMVector3LengthSq(line.end));
//	t = 0.0f;
//	if (lenSq > 0.0f)
//	{
//		t = Utility::VECtoFloat(XMVector3Dot(line.end, point - line.start)) / lenSq;
//	}
//
//	hit = line.start + t * line.end;
//	return Utility::VECtoFloat(XMVector3Length(hit - point));
//}
//
//float CollisionUtility::CalcPointSegmentDist(const Vector3& point, const LineInfo& line, Vector3& h, float& t)
//{
//	const Vector3 end = line.start + line.end;
//	LineInfo l;
//	l.start = line.start;
//	l.end = end - line.start;
//	float len = CalcPointLineDistance(point, l, h, t);
//
//	if (!IsSharpAngle3(point, line.start, end))
//	{
//		h = line.start;
//		return Utility::VECtoFloat(XMVector3Length(line.start - point));
//	}
//	else if (!IsSharpAngle3(point, end, line.start))
//	{
//		h = end;
//		return Utility::VECtoFloat(XMVector3Length(end - point));
//	}
//
//	return len;
//}
//
//float CollisionUtility::CalcLineLineDistance(const LineInfo& line1, const LineInfo& line2, Vector3& point1, Vector3& point2, float& t1, float& t2)
//{
//	if (IsParallel(line1.end, line2.end))
//	{
//		float len = CalcPointLineDistance(line1.start, line2, point2, t2);
//		point1 = line1.start;
//		t1 = 0.0f;
//		return len;
//	}
//
//	float dv1v2 = Utility::VECtoFloat(XMVector3Dot(line1.end, line2.end));
//	float dv1v1 = Utility::VECtoFloat(XMVector3LengthSq(line1.end));
//	float dv2v2 = Utility::VECtoFloat(XMVector3LengthSq(line2.end));
//	Vector3 p21p11 = line1.start - line2.start;
//	t1 = Utility::VECtoFloat((dv1v2 * XMVector3Dot(line2.end, p21p11) - dv2v2 * XMVector3Dot(line1.end, p21p11))) / (dv1v1 * dv2v2 - dv1v2 * dv1v2);
//	point1 = line1.start + t1 * line1.end;
//	t2 = Utility::VECtoFloat(XMVector3Dot(line2.end, point1 - line2.start)) / dv2v2;
//	point2 = line2.start + t2 * line2.end;
//
//
//	return Utility::VECtoFloat(XMVector3Length(point1 - point2));
//}
//
//bool CollisionUtility::IsSharpAngle(const Vector3& point1, const Vector3 point2)
//{
//	return (Utility::VECtoFloat(XMVector3Dot(point1,point2)) >= 0.0f);
//}
//
//bool CollisionUtility::IsSharpAngle3(const Vector3& point1, const Vector3 point2, const Vector3 point3)
//{
//	return IsSharpAngle(point1 - point2, point3 - point2);
//}
//
//bool CollisionUtility::IsParallel(const Vector3& v1, const Vector3& v2)
//{
//	float d = Utility::VECtoFloat(XMVector3Length(XMVector3Cross(v1, v2)));
//	return (-0.00001f < d && d < 0.00001f);
//}
//
//void CollisionUtility::Clamp01(float& v)
//{
//	if (v < 0.0f)
//		v = 0.0f;
//	else if (v > 1.0f)
//		v = 1.0f;
//}

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