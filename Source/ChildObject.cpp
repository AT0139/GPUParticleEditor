#include "ChildObject.h"
#include "Scene.h"
#include "Manager.h"
#include "Player.h"

ChildObject::ChildObject()
{
}

ChildObject::~ChildObject()
{
}

void ChildObject::Update()
{
}

void ChildObject::Draw()
{
}

void ChildObject::OnCollision(GameObject* collision)
{
	GetComponent<Transform>()->GetParent()->OnCollision(collision);
}

void ChildObject::OnTrigger(GameObject* collision)
{
	GetComponent<Transform>()->GetParent()->OnTrigger(collision);
}
