#pragma once

#include "GameObject.h"
#include "EnemyStateBase.h"

class Enemy :public GameObject
{
public:
	Enemy();
	~Enemy();
	virtual void Update() override;
	virtual void Draw() override {}

private:
	std::shared_ptr<EnemyStateBase> m_state;
};