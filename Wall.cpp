#include "Wall.h"
#include "DrawModel.h"
#include "OBBCollision.h"

Wall::Wall()
{
	auto transform = GetComponent<Transform>();
	transform->SetScale(Vector3(0.3f, 0.3f, 0.3f));

	auto rigid = AddComponent<Rigidbody>();
	rigid->SetIsKinematic(true);

	auto model = AddComponent<DrawModel>(this);
	model->Load("asset\\model\\Wall.obj");
	
	auto collision = AddComponent<OBBCollision>();
	collision->SetScale(Vector3(0.4f,5.0f,3.0f));
	collision->SetIsStaticObject(true);
}

Wall::~Wall()
{
}

void Wall::Update()
{
}

void Wall::Draw()
{
}
