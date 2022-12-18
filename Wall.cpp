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
#ifdef _DEBUG
	{
		ImGui::Begin("Wall");

		ImGui::Text("x = %f", GetComponent<Transform>()->GetPosition().x);
	

		if(ImGui::Button("serialize"))
		{
			Cereal();
		}
		ImGui::End();
	}
#endif
}

void Wall::Draw()
{
}

void Wall::Cereal()
{
	std::ofstream os("wall.json", std::ios::out);
	cereal::JSONOutputArchive archiveFile(os);
	serialize(archiveFile);
}
