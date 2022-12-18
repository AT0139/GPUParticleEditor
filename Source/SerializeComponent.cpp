#include "SerializeComponent.h"
#include "GameObject.h"
#include "Transform.h"

SerializeComponent::SerializeComponent(GameObject* pGameObject)
	: Component(pGameObject)
{}

SerializeComponent::~SerializeComponent()
{}

void SerializeComponent::Update()
{
}

void SerializeComponent::Draw()
{
}

SerializeData SerializeComponent::GetSerializeData()
{
	auto pObj = GetGameObject();
	auto transform = pObj->GetComponent<Transform>();
	SerializeData data(transform->GetPosition(), transform->GetRotation());

	return data;
}
