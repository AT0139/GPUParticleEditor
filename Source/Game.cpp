#include "main.h"
#include "Renderer.h"
#include "Game.h"
#include "Polygon2D.h"
#include "Camera.h"
#include "MeshField.h"
#include "Player.h"
#include "Stair.h"
#include "Bullet.h"
#include "Enemy.h"
#include "SkyDome.h"
#include "Tree.h"
#include "input.h"
#include "Manager.h"
#include "Result.h"
#include "ResourceManager.h"
#include "audio.h"
#include "Explosion.h"
#include "GameUI.h"

namespace MainGame
{
	void Game::Init()
	{
		//カメラ
		AddGameObject<Camera>(CAMERA);

		//3Dオブジェクト
		AddGameObject<SkyDome>(OBJECT);
		AddGameObject<Player>(OBJECT);
		AddGameObject<MeshField>(OBJECT);
		AddGameObject<Stair>(OBJECT)->GetComponent<Transform>()->SetPosition(Vector3(10.0f, 5.0f, 10.0f));
		AddGameObject<Enemy>(OBJECT)->GetComponent<Transform>()->SetPosition(Vector3(-8.0f, 4.0f, 3.0f));

		AddGameObject<GameUI>(UI);
		//Audio* bgm = AddGameObject<Audio>(UI);
		//bgm->Load("asset\\audio\\bgm.wav");
		//bgm->Play(true);

		m_collisionManager = std::make_unique<CollisionManager>();
	}

	void Game::Uninit()
	{
		Scene::Uninit();
		ResourceManager::GetInstance().Release();
	}

	void Game::Update()
	{
		Scene::Update();

		m_collisionManager->Update();
		if (Input::GetKeyTrigger(KEY_CONFIG::RETURN))
		{
			Manager::GetInstance().SetScene<Result>();
		}

#ifdef _DEBUG
		auto mousePos = GetMousePos();
		ImGui::Begin("mouse");
		{
			ImGui::Text("x = %d y = %d", mousePos.x, mousePos.y);
		}
		ImGui::End();
#endif 
	}
}