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
	ObjectPlacementUI* m_pPlacementUI;
	GameObject* m_pPlaceObject;
};