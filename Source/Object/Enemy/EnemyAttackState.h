#pragma once

#include "EnemyStateBase.h"

namespace MainGame
{
	class EnemyAttackState :public EnemyStateBase
	{
	public:
		EnemyAttackState(Enemy* pGO);
		virtual ~EnemyAttackState();

		// EnemyStateBase を介して継承されました
		virtual void StateEnter() override;
		virtual void StateUpdate() override;
		virtual void StateExit() override;

	private:
		void Shot();

		int m_cooldown;
	};
}