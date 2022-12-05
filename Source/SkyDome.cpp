#include "main.h"
#include "renderer.h"
#include "SkyDome.h"
#include "DrawModel.h"

SkyDome::SkyDome()
{
	//モデル読み込み
	AddComponent<DrawModel>(this)->Load("asset\\model\\skydome.obj");

	auto transform = GetComponent<Transform>();
	transform->SetPosition(D3DXVECTOR3(0.0f, -100.0f, 0.0f));
	transform->SetRotation(D3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f));
	transform->SetScale(D3DXVECTOR3(150.0f, 150.0f, 150.0f));
}

SkyDome::~SkyDome()
{
}

void SkyDome::Update()
{
}