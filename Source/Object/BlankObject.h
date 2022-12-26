#pragma once

#include "GameObject.h"

class BlankObject : public GameObject
{
public:
	BlankObject();
	~BlankObject();

	// GameObject を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

	void OnCollision(GameObject* collision)override;
	void OnTrigger(GameObject* collision)override;
};