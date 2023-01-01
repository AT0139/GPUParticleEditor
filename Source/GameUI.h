#pragma once

#include "GameObject.h"
#include "PlacementObjectSnap.h"


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


	void PlacementUIUpdate();
	void CreateObjectAtID(int staticObjectID);

	ObjectPlacementUI* m_pPlacementUI;
	Placement::PlacementObject* m_pPlaceObject;
	std::list<Placement::SnapObjectInfo> m_pSnapObjectList;

	std::shared_ptr<StaticObjectData> m_placeObjectData;
};