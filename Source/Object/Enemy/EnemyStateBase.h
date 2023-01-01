#pragma once

#include "GameObject.h"

class Enemy;

class EnemyStateBase
{
public:
	EnemyStateBase(Enemy* pGO) : m_pGameObject(pGO) {}
	virtual ~EnemyStateBase() {}

	virtual void StateEnter() = 0;
	virtual void StateUpdate() = 0;
	virtual void StateExit() = 0;

protected:
	Enemy* m_pGameObject;
};