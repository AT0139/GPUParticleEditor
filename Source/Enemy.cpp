#include "main.h"
#include "renderer.h"
#include "model.h"
#include "ResourceManager.h"
#include "Enemy.h"
#include "DrawModel.h"
#include "SphereCollision.h"
#include "Rigidbody.h"



Enemy::Enemy()
{
	AddComponent<DrawModel>(this)->Load("asset\\model\\torus.obj");
	AddComponent<SphereCollision>()->SetRadius(0.7f);
	auto rigid = AddComponent<Rigidbody>();

	auto transform = GetComponent<Transform>();
	transform->SetPosition(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	transform->SetRotation(D3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f));
	transform->SetScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f));
	
	m_tag = TAG::ENEMY;
}

Enemy::~Enemy()
{}

void Enemy::Update()
{
}