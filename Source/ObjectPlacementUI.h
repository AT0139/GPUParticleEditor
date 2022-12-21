#pragma once

#include "GameObject.h"

class Draw2DPolygon;

enum class OBJECT_TYPE
{
	WALL,
	FLOOR,

	NONE,
};

class ObjectPlacementUI : public GameObject
{
public:
	ObjectPlacementUI();
	~ObjectPlacementUI();

	// GameObject を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

	OBJECT_TYPE IsCreate() { return m_createObject; }
	void ResetIsCreate() { m_createObject = OBJECT_TYPE::NONE; }

private:
	OBJECT_TYPE m_createObject;

	std::shared_ptr<Draw2DPolygon> m_backGround;
	std::list <std::shared_ptr<Draw2DPolygon>> m_pIcons;
};