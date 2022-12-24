#include "main.h"
#include "renderer.h"
#include "SkyDome.h"
#include "DrawModel.h"
#include "Manager.h"
#include "Scene.h"
#include "Player.h"

SkyDome::SkyDome()
{
	//モデル読み込み
	auto model = AddComponent<DrawModel>(this);
	model->Load("asset\\model\\skydome.obj");
	model->SetVertexShader("unlitTextureVS.cso");
	model->SetPixelShader("unlitTexturePS.cso");

	auto transform = GetComponent<Transform>();
	transform->SetPosition(Vector3(0.0f, -100.0f, 0.0f));
	transform->SetQuaternion(Quaternion(0.0f, 0.0f, 0.0f, 0.0f));
	transform->SetScale(Vector3(250.0f, 250.0f, 250.0f));
}

SkyDome::~SkyDome()
{
}

void SkyDome::Update()
{
	auto scene = Manager::GetInstance().GetScene();
	auto player = scene->GetGameObject<MainGame::Player>(scene->OBJECT);

	auto playerPos = player->GetComponent<Transform>()->GetPosition();

	auto transform = GetComponent<Transform>();
	transform->SetPosition(Vector3(playerPos.x, playerPos.y+-100.0f, playerPos.z));
}