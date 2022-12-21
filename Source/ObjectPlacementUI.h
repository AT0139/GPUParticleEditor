#pragma once

#include "GameObject.h"

class Draw2DPolygon;


class ObjectPlacementUI : public GameObject
{
public:
	ObjectPlacementUI();
	~ObjectPlacementUI();

	// GameObject を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

	int GetCreateModelID() { return m_createModelID; }
	void ResetModelID() { m_createModelID = -1; }
private:
	struct IconInfo
	{
		int modelID;
		std::shared_ptr<Draw2DPolygon> icon;
	};

	void MoveIcons(float xValue);

	int m_createModelID;
	int m_selectIndex;
	float m_xMovingDistance;
	std::shared_ptr<Draw2DPolygon> m_backGround;
	std::shared_ptr<Draw2DPolygon> m_flame;
	std::vector<IconInfo> m_pIconInfos;
};