#include "ResourceManager.h"
#include "Enemy.h"
#include "DrawModel.h"
#include "Rigidbody.h"
#include "Manager.h"
#include "Scene.h"
#include "DefenceBase.h"
#include "ChildObject.h"

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
	
	auto obj = scene->AddGameObject<ChildObject>(scene->OBJECT);
	auto objRigid = obj->AddComponent<Rigidbody>();
	objRigid->SetIsTrigger(true);
	objRigid->SetIsKinematic(true);
	auto objCol = obj->AddComponent<SphereCollision>();
	objCol->SetRadius(0.1f);
	objCol->SetHitAction(HitAction::None);
	auto objTrans = obj->GetComponent<Transform>();
	objTrans->SetParent(this);
	objTrans->SetPosition(-transform->GetForward() * 1.0f);

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