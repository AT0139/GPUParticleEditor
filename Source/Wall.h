#pragma once

#include "GameObject.h"

class Wall : public GameObject
{
public:
	Wall();
	~Wall();

	// GameObject を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

private:
};