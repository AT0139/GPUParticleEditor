#pragma once

#include "GameObject.h"

class ObjectPlacementUI;

class GameUI : public GameObject
{
public:
	GameUI();
	~GameUI();

	// GameObject を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

private:
	struct SnapObjectInfo
	{
		BoundingOrientedBox m_obb;
		std::vector<Vector3> m_snapPoint;
	};

	void PlacementUIUpdate();
	void CreateObjectAtID(int staticObjectID);
	SnapObjectInfo CreateSnapInfo();

	ObjectPlacementUI* m_pPlacementUI;
	GameObject* m_pPlaceObject;
	std::list<SnapObjectInfo> m_pSnapObjectList;

	std::shared_ptr<StaticObjectData> m_placeObjectData;
};