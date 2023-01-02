#pragma once

#include "GameObject.h"

class Turret : GameObject
{
public:
	Turret();
	~Turret();

	// GameObject を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

private:
};