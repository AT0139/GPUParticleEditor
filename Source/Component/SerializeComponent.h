#pragma once

#include "Component.h"
#include "GameObject.h"

struct SerializeData
{
	SerializeData() {}
	SerializeData(Vector3 pos, Quaternion rotate)
		: positionX(pos.x), positionY(pos.y), positionZ(pos.z)
		, rotateX(rotate.x), rotateY(rotate.y), rotateZ(rotate.z), rotateW(rotate.w)
	{}

	float positionX, positionY, positionZ;
	float rotateX, rotateY, rotateZ, rotateW;
};


class SerializeComponent : public Component
{
public:
	SerializeComponent(GameObject* pGameObject);
	virtual ~SerializeComponent();

	// Component を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

	SerializeData GetSerializeData();
private:
};