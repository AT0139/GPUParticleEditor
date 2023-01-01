#include "ResourceManager.h"
#include "Enemy.h"
#include "DrawModel.h"
#include "Rigidbody.h"
#include "Manager.h"
#include "Scene.h"
#include "DefenceBase.h"
#include "ChildObject.h"
#include "EnemyMoveToTarget.h"

Enemy::Enemy()
{
	AddComponent<DrawModel>(this)->Load("asset\\model\\Enemy.obj");
	AddComponent<OBBCollision>()->SetScale(0.5f);
	AddComponent<Rigidbody>();

	auto transform = GetComponent<Transform>();
	transform->SetQuaternion(Quaternion(0.0f, 0.0f, 0.0f, 0.0f));
	transform->SetScale(Vector3(0.5f, 0.5f, 0.5f));


	SetTag(TAG::ENEMY);

	m_state = std::make_shared<EnemyMoveToTarget>(EnemyMoveToTarget(this));
	m_state->StateEnter();
}

Enemy::~Enemy()
{}

void Enemy::Update()
{
	m_state->StateUpdate();

}