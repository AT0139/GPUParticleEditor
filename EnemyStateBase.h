#pragma once

#include "GameObject.h"

class EnemyStateBase
{
public:
	EnemyStateBase(GameObject* pGO) : m_pGameObject(pGO) {}
	virtual ~EnemyStateBase() {}

	virtual void StateEnter() = 0;
	virtual void StateUpdate() = 0;
	virtual void StateExit() = 0;

protected:
	GameObject* m_pGameObject;
};