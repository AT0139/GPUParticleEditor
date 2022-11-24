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
		transform->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		transform->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		transform->SetScale(D3DXVECTOR3(2.0f, 2.0f, 2.0f));
	}

	Tree::~Tree()
	{}

	void Tree::Update()
	{
	}

	void Tree::Draw()
	{}
}