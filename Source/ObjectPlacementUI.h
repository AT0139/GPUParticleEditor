#pragma once

#include "GameObject.h"

class Draw2DPolygon;
class Enemy;

class ObjectPlacementUI : public GameObject
{
public:
	ObjectPlacementUI();
	~ObjectPlacementUI();

	// GameObject を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

	bool IsCreate() { return m_isCreate; }
	void ResetIsCreate() { m_isCreate = false; }

private:
	bool m_isCreate;

	std::shared_ptr<Draw2DPolygon> m_backGround;
	std::shared_ptr<Draw2DPolygon> m_icon;
};