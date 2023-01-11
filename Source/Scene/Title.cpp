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
	GET_INPUT.ToggleCursor(true);

	//カメラ
	SetCamera<TitleCamera>();
	AddGameObject<SkyDome>(OBJECT)->GetComponent<Transform>()->SetPosition(Vector3(0.0f, -200.0f, 0.0f));

	AddGameObject<MainGame::DefenceBase>(OBJECT)->GetComponent<Transform>();

	auto particleManager = AddGameObject<ParticleEmitterManager>(EFFECT);
	particleManager->GetComponent<Transform>()->SetPosition(Vector3(0.0f, -40.0f, 0.0f));

	EmitterInitData initData = {};
	initData.size = Vector2(1.0f, 1.0f);
	initData.direction = Vector3::Up * 0.0001f;
	initData.life = 300;
	initData.filePath = L"Asset\\Texture\\BlueBloom.png";
	initData.maxNum = 100000;
	initData.createInterval = 10;
	initData.createOnceNum = 50;
	particleManager->AddEmitter(initData);
	initData.size = Vector2(0.3f, 0.3f);
	initData.direction = -Vector3::Up * 0.001f;
	initData.life = 500;
	initData.filePath = L"Asset\\Texture\\WhiteBloom.png";
	initData.maxNum = 100000;
	initData.createInterval = 50;
	initData.createOnceNum = 5000;
	particleManager->AddEmitter(initData);
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