#pragma once

#include "GameObject.h"

class Floor : public GameObject
{
public:
	Floor();
	~Floor();

	// GameObject を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

private:
};