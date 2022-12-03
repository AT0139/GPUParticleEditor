#include "main.h"
#include "renderer.h"
#include "model.h"
#include "ResourceManager.h"
#include "Enemy.h"
#include "DrawModel.h"
#include "OBBCollision.h"
#include "Rigidbody.h"
#include "AABBCollision.h"

shared_ptr<Rigidbody> rigid;

Enemy::Enemy()
{
	AddComponent<DrawModel>(this)->Load("asset\\model\\torus.obj");
	AddComponent<OBBCollision>();
	rigid = AddComponent<Rigidbody>();


	auto transform = GetComponent<Transform>();
	transform->SetPosition(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	transform->SetRotation(D3DXVECTOR3(0.0f, 90.0f, 0.0f));
	transform->SetScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f));

	m_tag = TAG::ENEMY;
}

Enemy::~Enemy()
{}

void Enemy::Update()
{
	ImGui::Begin("General");
	{
		if (ImGui::Button("Velo"))
		{
			rigid->AddForce(D3DXVECTOR3(1.0f, 0.0f, 0.0f));
		}
	
	}
	ImGui::End();
}