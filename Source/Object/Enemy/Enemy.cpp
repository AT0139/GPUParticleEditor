#include "ResourceManager.h"
#include "Enemy.h"
#include "DrawModel.h"
#include "Rigidbody.h"
#include "Manager.h"
#include "Scene.h"
#include "DefenceBase.h"
#include "EnemyMoveToTargetState.h"
#include "EnemyAttackState.h"

namespace MainGame
{
	Enemy::Enemy()
	{
		AddComponent<DrawModel>(this)->Load("asset\\model\\Enemy.obj");
		AddComponent<OBBCollision>()->SetScale(0.5f);
		AddComponent<Rigidbody>();

		m_transform = GetComponent<Transform>();
		m_transform->SetQuaternion(Quaternion(0.0f, 0.0f, 0.0f, 0.0f));
		m_transform->SetScale(Vector3(0.5f, 0.5f, 0.5f));


		SetTag(TAG::ENEMY);

		m_state = std::make_shared<EnemyMoveToTargetState>(EnemyMoveToTargetState(this));
		m_state->StateEnter();

		auto scene = Manager::GetInstance().GetScene();
		m_targetPos = scene->GetGameObject<DefenceBase>(scene->OBJECT)->GetComponent<Transform>()->GetPosition();
	}

	Enemy::~Enemy()
	{}

	void Enemy::Update()
	{
		m_state->StateUpdate();
	}
}