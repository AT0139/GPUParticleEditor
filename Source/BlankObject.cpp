#include "BlankObject.h"
#include "Scene.h"
#include "Manager.h"
#include "Player.h"

BlankObject::BlankObject()
{
}

BlankObject::~BlankObject()
{
}

void BlankObject::Update()
{
}

void BlankObject::Draw()
{
}

void BlankObject::OnCollision(GameObject* collision)
{
	ImGui::Begin("General");
	{
		ImGui::Text("GrabCollision!");
	}
	ImGui::End();
	GetComponent<Transform>()->GetParent()->OnCollision(collision);
}

void BlankObject::OnTrigger(GameObject* collision)
{
	ImGui::Begin("General");
	{
		ImGui::Text("GrabTrigger!");
	}
	ImGui::End();
	GetComponent<Transform>()->GetParent()->OnTrigger(collision);
}
