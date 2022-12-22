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
	void PlacementUIUpdate();
	void CreateObjectAtID(int modelID);

	ObjectPlacementUI* m_pPlacementUI;
	GameObject* m_pPlaceObject;
	std::list<std::shared_ptr<BoundingOrientedBox>> m_pSnapPositionList;
};