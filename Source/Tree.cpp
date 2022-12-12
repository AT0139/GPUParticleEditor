#include "main.h"
#include "renderer.h"
#include "Tree.h"
#include "manager.h"
#include "Scene.h"
#include "DrawBillboard.h"
#include "ResourceManager.h"

namespace MainGame
{
	Tree::Tree()
	{
		AddComponent<DrawBillboard>(this)->Load("asset/texture/Tree.png");

		auto transform = GetComponent<Transform>();
		transform->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
		transform->SetQuaternion(Quaternion(0.0f, 0.0f, 0.0f, 0.0f));
		transform->SetScale(Vector3(2.0f, 2.0f, 2.0f));
	}

	Tree::~Tree()
	{}

	void Tree::Update()
	{
	}

	void Tree::Draw()
	{}
}