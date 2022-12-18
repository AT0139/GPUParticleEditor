#include "Floor.h"
#include "SerializeComponent.h"

Floor::Floor()
{
	auto transform = GetComponent<Transform>();
	transform->SetScale(Vector3(1.5f,1.5f,1.5f));

	auto rigid = AddComponent<Rigidbody>();
	rigid->SetIsKinematic(true);

	auto model = AddComponent<DrawModel>(this);
	model->Load("asset\\model\\Floor.obj");

	auto collision = AddComponent<OBBCollision>();
	collision->SetScale(Vector3(3.0f, 0.3f, 3.0f));
	collision->SetIsStaticObject(true);

	AddComponent<SerializeComponent>(this);

	m_tag = TAG::FLOOR;
}

Floor::~Floor()
{
}

void Floor::Update()	
{
}

void Floor::Draw()
{
}
