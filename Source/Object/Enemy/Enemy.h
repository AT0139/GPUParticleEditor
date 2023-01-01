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

	template<class T>
	void StateTransition();

	Vector3 GetTargetPos() { return m_targetPos; }
private:
	std::shared_ptr<EnemyStateBase> m_state;
	std::shared_ptr<Transform> m_transform;
	Vector3 m_targetPos;
};

template<class T>
inline void Enemy::StateTransition()
{
	m_state->StateExit();
	m_state = std::make_shared<T>(T(this));
	m_state->StateEnter();
}
