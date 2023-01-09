#include "Title.h"
#include "Input.h"
#include "Manager.h"
#include "Camera.h"
#include "Game.h"
#include "ParticleEmitterManager.h"
#include "DefenceBase.h"
#include "SkyDome.h"

void Title::Init()
{
	//カメラ
	SetCamera<Camera>();
	AddGameObject<SkyDome>(OBJECT)->GetComponent<Transform>()->SetPosition(Vector3(0.0f, -200.0f, 0.0f));

	AddGameObject<MainGame::DefenceBase>(OBJECT)->GetComponent<Transform>();
	AddGameObject<ParticleEmitterManager>(EFFECT)->GetComponent<Transform>()->SetPosition(Vector3(0.0f,-40.0f,0.0f));

}

void Title::Uninit()
{
	Scene::Uninit();
}

void Title::Update()
{
	Scene::Update();

	if (GET_INPUT.GetKeyTrigger(KEY_CONFIG::RETURN))
	{
		Manager::GetInstance().SetScene<MainGame::Game>();
	}
}