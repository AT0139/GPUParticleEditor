#include "main.h"
#include "renderer.h"
#include "model.h"
#include "ResourceManager.h"
#include "Stair.h"
#include "DrawModel.h"

Stair::Stair()
{
	AddComponent<DrawModel>(this)->Load("asset\\model\\upstair.obj");

	auto transform = GetComponent<Transform>();
	AddComponent<Rigidbody>()->SetIsKinematic(true);
	auto col = AddComponent<OBBCollision>();
	col->SetScale(4.0f);
	col->SetIsStaticObject(true);
	//transform->SetPosition(Vector3(0.0f, 5.0f, 0.0f));
	//transform->SetRotation(Quaternion(0.0f, 0.0f, 0.0f, 0.0f));
	//transform->SetScale(Vector3(0.3f, 0.3f, 0.3f));
}

Stair::~Stair()
{
}

void Stair::Update()
{
}