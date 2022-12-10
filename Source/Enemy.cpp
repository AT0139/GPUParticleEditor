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
	AddComponent<AABBCollision>()->SetScale(Vector3(0.7f, 0.7f, 0.7f));
	AddComponent<Rigidbody>();

	auto transform = GetComponent<Transform>();
	transform->SetPosition(Vector3(0.0f, 1.0f, 0.0f));
	transform->SetRotation(Quaternion(0.0f, 0.0f, 0.0f, 0.0f));
	transform->SetScale(Vector3(0.5f, 0.5f, 0.5f));

	m_tag = TAG::ENEMY;
}

Enemy::~Enemy()
{}

void Enemy::Update()
{
	ImGui::Begin("General");
	{
		ImGui::Text("x = %f y = %f z = %f", GetComponent<Transform>()->GetPosition().x,GetComponent<Transform>()->GetPosition().y, GetComponent<Transform>()->GetPosition().z);

	}
	ImGui::End();
}