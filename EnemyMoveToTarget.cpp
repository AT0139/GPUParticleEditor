#include "EnemyMoveToTarget.h"
#include "GameObject.h"
#include "Manager.h"
#include "Scene.h"
#include "DefenceBase.h"
#include "MeshField.h"

static const float MOVE_SPEED = 0.05f;


EnemyMoveToTarget::EnemyMoveToTarget(GameObject* pGO)
	: EnemyStateBase(pGO)
	, m_aster(false)
	, m_wayPointIndex(0)
{
}

EnemyMoveToTarget::~EnemyMoveToTarget()
{
}

void EnemyMoveToTarget::StateEnter()
{
	auto scene = Manager::GetInstance().GetScene();
	m_targetPos = scene->GetGameObject<DefenceBase>(scene->OBJECT)->GetComponent<Transform>()->GetPosition();
	
}

void EnemyMoveToTarget::StateUpdate()
{
	auto myPos = m_pGameObject->GetComponent<Transform>()->GetPosition();

	if (!m_aster)
	{
		CalcWayPoint(myPos, m_targetPos);
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
}

void EnemyMoveToTarget::StateExit()
{

}

void EnemyMoveToTarget::CalcWayPoint(Vector3 startPos, Vector3 endPos)
{
	auto scene = Manager::GetInstance().GetScene();
	auto field = scene->GetGameObject<MainGame::MeshField>(scene->OBJECT);

	auto pStartNode = field->GetNodes(startPos);
	auto pEndNode = field->GetNodes(endPos);

	Node* pNode = pStartNode;
	float total = 0;
	while (pNode != pEndNode)
	{
		Node* nextNode = nullptr;
		float minScore = 99999;
		//次のノードのスコア計算
		for (auto adjacent : pNode->adjacentNode)
		{
			if (adjacent.node->notTraffic)
				continue;

			auto estimated = endPos - adjacent.node->pos;
			float score = total + adjacent.cost + estimated.Length();
			if (minScore > score)
			{
				minScore = score;
				nextNode = adjacent.node;
			}
		}

		m_wayPoint.push_back(nextNode->pos);
		pNode = nextNode;
	}
}
