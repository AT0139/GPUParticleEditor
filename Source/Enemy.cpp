#include "ResourceManager.h"
#include "Enemy.h"
#include "DrawModel.h"
#include "Rigidbody.h"
#include "Manager.h"
#include "Scene.h"
#include "DefenceBase.h"

static const float MOVE_SPEED = 0.07f;

Enemy::Enemy()
{
	AddComponent<DrawModel>(this)->Load("asset\\model\\Enemy.obj");
	AddComponent<OBBCollision>()->SetScale(0.5f);
	AddComponent<Rigidbody>();

	auto transform = GetComponent<Transform>();
	transform->SetPosition(Vector3(0.0f, 1.0f, 0.0f));
	transform->SetQuaternion(Quaternion(0.0f, 0.0f, 0.0f, 0.0f));
	transform->SetScale(Vector3(0.5f, 0.5f, 0.5f));

	auto scene = Manager::GetInstance().GetScene();
	m_targetPos = scene->GetGameObject<DefenceBase>(scene->OBJECT)->GetComponent<Transform>()->GetPosition();
	

	m_tag = TAG::ENEMY;
}

Enemy::~Enemy()
{}

void Enemy::Update()
{
	auto myPos = GetComponent<Transform>()->GetPosition();

	auto targetDir= m_targetPos - myPos;
	targetDir.Normalize();

	GetComponent<Rigidbody>()->SetVelocity(targetDir * MOVE_SPEED);


}