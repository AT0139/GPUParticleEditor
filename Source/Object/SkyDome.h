#pragma once

#include "GameObject.h"

class SkyDome :public GameObject
{
public:
	SkyDome();
	~SkyDome();
	virtual void Update() override;
	virtual void Draw() override {}

private:
};