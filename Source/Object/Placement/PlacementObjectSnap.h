#pragma once

#include "GameObject.h"

namespace Placement
{

	static const float ROTATION_SPEED = 0.05f;
	class PlacementObject;
	class Foundation;
	class Wall;

	enum class Axiz
	{
		X,
		Y,
		Z
	};

	struct SnapPoint
	{
		Vector3 position;
		Axiz axiz;
	};

	struct SnapObjectInfo
	{
		BoundingOrientedBox m_obb;
		std::vector<SnapPoint> m_snapPointList;
		PlacementObject* m_obj;
	};


	class PlacementObject : public GameObject
	{
	public:
		PlacementObject();
		~PlacementObject() {}

		// GameObject を介して継承されました
		virtual void Update() override {}
		virtual void Draw() override {}

		virtual void SetSnapPoint(Foundation* obj, SnapObjectInfo info, Vector3 hitPos) = 0;
		virtual void SetSnapPoint(Wall* obj, SnapObjectInfo info, Vector3 hitPos) = 0;

		virtual SnapObjectInfo CreateSnapInfo() = 0;

		virtual void SnapBridge(PlacementObject* obj, SnapObjectInfo info, Vector3 hitPos) = 0;

	protected:
		SnapPoint NearestSnapPoint(std::vector<SnapPoint> snapPointList, Vector3 position);
	};
}