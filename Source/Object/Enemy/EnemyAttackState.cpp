#include "EnemyAttackState.h"
#include "Bullet.h"
#include "Manager.h"
#include "Scene.h"
#include "Enemy.h"

static const int ATTACK_COOLTIME = 60;

namespace MainGame
{
	EnemyAttackState::EnemyAttackState(Enemy* pGO)
		: EnemyStateBase(pGO)
		, m_cooldown(ATTACK_COOLTIME)
	{
	}

	EnemyAttackState::~EnemyAttackState()
	{
	}

	void EnemyAttackState::StateEnter()
	{
	}

	void EnemyAttackState::StateUpdate()
	{
		m_cooldown++;
		if (m_cooldown >= ATTACK_COOLTIME)
		{
			Shot();
			m_cooldown = 0;
		}
	}

	void EnemyAttackState::StateExit()
	{
	}

	void EnemyAttackState::Shot()
	{
		auto myPos = m_pGameObject->GetComponent<Transform>()->GetPosition();
		Vector3 targetPos = m_pGameObject->GetTargetPos();
		auto dir = targetPos - myPos;
		dir.Normalize();

		auto scene = Manager::GetInstance().GetScene();
		auto bullet = scene->AddGameObject<MainGame::Bullet>(scene->OBJECT);
		bullet->GetComponent<Transform>()->SetPosition(m_pGameObject->GetComponent<Transform>()->GetPosition());
		bullet->SetVelocity(dir);

	}
}