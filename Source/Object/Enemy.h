#pragma once

#include "GameObject.h"

class Enemy :public GameObject
{
public:
	Enemy();
	~Enemy();
	virtual void Update() override;
	virtual void Draw() override {}

private:
	Vector3 m_targetPos;
};