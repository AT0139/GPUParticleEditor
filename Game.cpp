#include "main.h"
#include "Game.h"
#include "Polygon2D.h"
#include "Camera.h"
#include "Field.h"
#include "Player.h"
#include "Stair.h"
#include "Bullet.h"
#include "Enemy.h"
#include "SkyDome.h"
#include "Tree.h"

void Game::Init()
{
	//カメラ
	AddGameObject<Camera>(CAMERA);

	//3Dオブジェクト
	AddGameObject<SkyDome>(OBJECT);
	AddGameObject<Player>(OBJECT);
	AddGameObject<Field>(OBJECT);
	AddGameObject<Stair>(OBJECT)->SetPosition(D3DXVECTOR3(10.0f, 0.0f, 10.0f));
	AddGameObject<Enemy>(OBJECT)->SetPosition(D3DXVECTOR3(-2.0f, 1.0f, 3.0f));
	AddGameObject<Enemy>(OBJECT)->SetPosition(D3DXVECTOR3(0.0f, 1.0f, 3.0f));
	AddGameObject<Enemy>(OBJECT)->SetPosition(D3DXVECTOR3(2.0f, 1.0f, 3.0f));
	AddGameObject<Tree>(OBJECT)->SetPosition(D3DXVECTOR3(-8.0f, 2.0f, 5.0f));

	//2Dオブジェクト
	AddGameObject<Polygon2D>(UI);
}

void Game::Uninit()
{
	Scene::Uninit();
}

void Game::Update()
{
	Scene::Update();
}
