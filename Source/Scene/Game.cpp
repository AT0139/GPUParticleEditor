﻿#include "main.h"
#include "Renderer.h"
#include "Game.h"
#include "MainCamera.h"
#include "MeshField.h"
#include "Player.h"
#include "SkyDome.h"
#include "input.h"
#include "SceneManager.h"
#include "Result.h"
#include "ResourceManager.h"
#include "Field.h"
#include "ParticleEmitterManager.h"

namespace MainGame
{
	void Game::Init()
	{
		GET_INPUT.ToggleCursor(false);

		//カメラ
		SetCamera<MainCamera>();

		//3Dオブジェクト
		AddGameObject<SkyDome>(OBJECT);
		//auto field = AddGameObject<MeshField>(OBJECT);

		AddGameObject<Player>(OBJECT);
		AddGameObject<Field>(OBJECT);

		auto particle = AddGameObject<ParticleEmitterManager>(EFFECT);
		particle->AddParticle("Fountain");
		particle->GetComponent<Transform>()->SetPosition(Vector3(0.1f, 15.0f, 0.1f));
		particle->GetComponent<Transform>()->SetScale(Vector3(0.1f, 0.1f, 0.1f));

	}

	void Game::Uninit()
	{
		Scene::Uninit();
		ResourceManager::GetInstance().Release();
	}

	void Game::Update()
	{
		Scene::Update();

		if (GET_INPUT.GetKeyTrigger(KEY_CONFIG::RETURN))
		{
			SceneManager::GetInstance().SetScene<Result>();
		}
	}
}