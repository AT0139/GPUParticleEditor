#include "Title.h"
#include "Polygon2D.h"
#include "input.h"
#include "manager.h"
#include "Game.h"

void Title::Init()
{
	AddGameObject<Polygon2D>(UI);
}

void Title::Uninit()
{
	Scene::Uninit();
}

void Title::Update()
{
	Scene::Update();

	if (Input::GetKeyTrigger(KEY_CONFIG::RETURN))
	{
		Manager::GetInstance().SetScene<MainGame::Game>();
	}
}
