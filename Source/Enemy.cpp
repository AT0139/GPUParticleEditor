#include "main.h"
#include "renderer.h"
#include "model.h"
#include "ResourceManager.h"
#include "Enemy.h"
#include "DrawModel.h"

Enemy::Enemy()
{
	AddComponent<DrawModel>(this)->Load("asset\\model\\torus.obj");

	auto transform = GetComponent<Transform>();
	transform->SetPosition(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	transform->SetRotation(D3DXVECTOR3(0.0f, 90.0f, 0.0f));
	transform->SetScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f));
}

Enemy::~Enemy()
{}

void Enemy::Update()
{}