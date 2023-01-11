#include "Title.h"
#include "Input.h"
#include "SceneManager.h"
#include "TitleCamera.h"
#include "Game.h"
#include "ParticleEmitterManager.h"
#include "DefenceBase.h"
#include "SkyDome.h"

void Title::Init()
{
	//カメラ
	SetCamera<TitleCamera>();
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
		SceneManager::GetInstance().SetScene<MainGame::Game>();
	}
}