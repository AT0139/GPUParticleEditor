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
		AddGameObject<Stair>(OBJECT)->GetComponent<Transform>()->SetPosition(D3DXVECTOR3(10.0f, 5.0f, 10.0f));
		AddGameObject<Enemy>(OBJECT)->GetComponent<Transform>()->SetPosition(D3DXVECTOR3(-2.0f, 5.0f, 3.0f));
		AddGameObject<Enemy>(OBJECT)->GetComponent<Transform>()->SetPosition(D3DXVECTOR3(0.0f, 5.0f, 3.0f));
		AddGameObject<Enemy>(OBJECT)->GetComponent<Transform>()->SetPosition(D3DXVECTOR3(2.0f, 5.0f, 3.0f));
		AddGameObject<Tree>(OBJECT)->GetComponent<Transform>()->SetPosition(D3DXVECTOR3(-0.0f, 5.0f, 0.0f));

		//2Dオブジェクト
		//AddGameObject<Polygon2D>(UI);

		//Audio* bgm = AddGameObject<Audio>(UI);
		//bgm->Load("asset\\audio\\bgm.wav");
		//bgm->Play(true);
	}

	void Game::Uninit()
	{
		Scene::Uninit();
		ResourceManager::GetInstance().Release();
	}

	void Game::Update()
	{
		Scene::Update();

		if (Input::GetKeyTrigger(KEY_CONFIG::RETURN))
		{
			Manager::GetInstance().SetScene<Result>();
		}
	}
}