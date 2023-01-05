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

	void InOutUI(bool isIn);

	int GetCreateStaticObjectID() { return m_createModelID; }
	void ResetModelID() { m_createModelID = -1; }

private:
	struct IconInfo
	{
		int modelID;
		std::shared_ptr<Draw2DPolygon> icon;
	};

	void MoveIcons(float value);
	void MoveAlltoXPositionOffset();

	bool m_isActive;

	bool m_isInOutMoving;
	bool m_isIn;
	float m_xPositionOffset;

	int m_createModelID;
	int m_selectIndex;
	float m_yMovingDistance;

	std::shared_ptr<Draw2DPolygon> m_backGround;
	std::shared_ptr<Draw2DPolygon> m_flame;
	std::vector<IconInfo> m_pIconInfos;
};