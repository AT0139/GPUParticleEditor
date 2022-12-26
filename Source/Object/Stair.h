#pragma once

#include "GameObject.h"

class Stair : public GameObject
{
public:
	Stair();
	~Stair();
	virtual void Update()override;
	virtual void Draw()override {}

private:
};