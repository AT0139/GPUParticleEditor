#pragma once

#include "EnemyStateBase.h"

class EnemyMoveToTargetState :public EnemyStateBase
{
public:
	EnemyMoveToTargetState(Enemy* pGO);
	  virtual ~EnemyMoveToTargetState();

	// EnemyStateBase を介して継承されました
	virtual void StateEnter() override;
	virtual void StateUpdate() override;
	virtual void StateExit() override;

	void CalcWayPoint(Vector3 startPos,Vector3 endPos);
private:

	std::vector<Vector3> m_wayPoint;
	int m_wayPointIndex;
	bool m_aster;
};