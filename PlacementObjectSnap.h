#pragma once

#include "GameObject.h"

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
	PlacementObject() {}
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

class Foundation : public PlacementObject
{
public:
	Foundation() {}
	~Foundation() {}

	// PlacementObject を介して継承されました
	virtual void SetSnapPoint(Foundation* obj, SnapObjectInfo info, Vector3 hitPos) override;
	virtual void SetSnapPoint(Wall* obj, SnapObjectInfo info, Vector3 hitPos) override;

	virtual SnapObjectInfo CreateSnapInfo() override;

	virtual void SnapBridge(PlacementObject* obj, SnapObjectInfo info, Vector3 hitPos) override;
};

class Wall : public PlacementObject
{
public:
	Wall() {}
	~Wall() {}

	// PlacementObject を介して継承されました
	virtual void SetSnapPoint(Foundation* obj, SnapObjectInfo info, Vector3 hitPos) override;
	virtual void SetSnapPoint(Wall* obj, SnapObjectInfo info, Vector3 hitPos) override;

	virtual SnapObjectInfo CreateSnapInfo() override;

	virtual void SnapBridge(PlacementObject* obj, SnapObjectInfo info, Vector3 hitPos) override;

};