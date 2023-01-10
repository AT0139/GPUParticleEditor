#include "EnemyMoveToTargetState.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "DefenceBase.h"
#include "MeshField.h"
#include "Enemy.h"
#include "EnemyAttackState.h"

static const float MOVE_SPEED = 0.05f;

namespace MainGame
{
	EnemyMoveToTargetState::EnemyMoveToTargetState(Enemy* pGO)
		: EnemyStateBase(pGO)
		, m_aster(false)
		, m_wayPointIndex(0)
	{
	}

	EnemyMoveToTargetState::~EnemyMoveToTargetState()
	{
	}

	void EnemyMoveToTargetState::StateEnter()
	{
		auto scene = SceneManager::GetInstance().GetScene();
		m_aster = false;
	}

	void EnemyMoveToTargetState::StateUpdate()
	{
		auto myPos = m_pGameObject->GetComponent<Transform>()->GetPosition();
		Vector3 targetPos = m_pGameObject->GetTargetPos();

		if (!m_aster)
		{
			CalcWayPoint(myPos, targetPos);
			m_aster = true;
		}

		auto targetDir = m_wayPoint[m_wayPointIndex] - myPos;
		//ウェイポイントに近くなったら次のウェイポイントに向かう
		if (1.0f >= targetDir.Length())
		{
			if (m_wayPoint.size() - 1 > m_wayPointIndex)
				m_wayPointIndex++;
		}
		targetDir.Normalize();


		m_pGameObject->GetComponent<Rigidbody>()->SetVelocity(targetDir * MOVE_SPEED);

		auto dir = targetPos - myPos;
		if (dir.Length() < 10.0f)
		{
			m_pGameObject->StateTransition<EnemyAttackState>();
		}
	}

	void EnemyMoveToTargetState::StateExit()
	{

	}

	void EnemyMoveToTargetState::CalcWayPoint(Vector3 startPos, Vector3 endPos)
	{
		auto scene = SceneManager::GetInstance().GetScene();
		auto field = scene->GetGameObject<MainGame::MeshField>(scene->OBJECT);

		auto pStartNode = field->GetNodes(startPos);
		auto pEndNode = field->GetNodes(endPos);

		Node* pNode = pStartNode;
		float total = 0;
		while (pNode != pEndNode)
		{
			Node* nextNode = nullptr;
			float minScore = -1;
			float addTotal = 0;
			//次のノードのスコア計算
			for (auto adjacent : pNode->adjacentNode)
			{
				if (adjacent.node->notTraffic)
					continue;

				auto Manhattan = std::abs(endPos.x - adjacent.node->pos.x) + std::abs(endPos.z - adjacent.node->pos.z);
				float score = total + adjacent.cost + Manhattan;
				if (minScore > score || minScore == -1)
				{
					minScore = score;
					nextNode = adjacent.node;
					addTotal = adjacent.cost;
				}
			}
			total += addTotal;
			m_wayPoint.push_back(nextNode->pos);
			pNode = nextNode;
		}
	}
}