﻿#pragma once

#include "GameObject.h"

class DrawAnimationModel;

class DefenceBase : public GameObject
{
public:
	DefenceBase();
	~DefenceBase();
	virtual void Update()override;
	virtual void Draw()override;

	virtual void OnTrigger(GameObject* collision) override;
private:
	std::shared_ptr<DrawAnimationModel> m_model;

};