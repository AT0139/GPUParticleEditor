#pragma once

#include "EnemyStateBase.h"

class EnemyMoveToTarget :public EnemyStateBase
{
public:
	EnemyMoveToTarget(GameObject* pGO);
	  virtual ~EnemyMoveToTarget();

	// EnemyStateBase を介して継承されました
	virtual void StateEnter() override;
	virtual void StateUpdate() override;
	virtual void StateExit() override;

	void CalcWayPoint(Vector3 startPos,Vector3 endPos);
private:
	Vector3 m_targetPos;
	std::vector<Vector3> m_wayPoint;
	int m_wayPointIndex;
	bool m_aster;
};