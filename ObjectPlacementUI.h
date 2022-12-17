#pragma once

#include "GameObject.h"

class Draw2DPolygon;
class DrawModel;

class ObjectPlacementUI : public GameObject
{
public:
	ObjectPlacementUI();
	~ObjectPlacementUI();

	// GameObject を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

private:
	std::shared_ptr<Draw2DPolygon> m_backGround;
	std::shared_ptr<DrawModel> m_obj;
};