#include "main.h"
#include "renderer.h"
#include "SkyDome.h"
#include "DrawModel.h"

SkyDome::SkyDome()
{
	//モデル読み込み
	AddComponent<DrawModel>(this)->Load("asset\\model\\skydome.obj");

	auto transform = GetComponent<Transform>();
	transform->SetPosition(Vector3(0.0f, -100.0f, 0.0f));
	transform->SetQuaternion(Quaternion(0.0f, 0.0f, 0.0f, 0.0f));
	transform->SetScale(Vector3(150.0f, 150.0f, 150.0f));
}

SkyDome::~SkyDome()
{
}

void SkyDome::Update()
{
}