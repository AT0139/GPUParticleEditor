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
	transform->SetPosition(D3DXVECTOR3(0.0f, 5.0f, 0.0f));
	transform->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI + 1.0f, 0.0f));
	transform->SetScale(D3DXVECTOR3(0.3f, 0.3f, 0.3f));
}

Stair::~Stair()
{
}

void Stair::Update()
{

}