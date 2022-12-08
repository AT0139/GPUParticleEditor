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
}

void BlankObject::OnTrigger(GameObject* collision)
{

}
