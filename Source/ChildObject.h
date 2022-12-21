#pragma once

#include "GameObject.h"

class ChildObject : public GameObject
{
public:
	ChildObject();
	~ChildObject();

	// GameObject を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

	void OnCollision(GameObject* collision)override;
	void OnTrigger(GameObject* collision)override;
};