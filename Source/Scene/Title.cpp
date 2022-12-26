#include "Title.h"
#include "BlankObject.h"
#include "input.h"
#include "manager.h"
#include "Game.h"
#include "Draw2DPolygon.h"

void Title::Init()
{
	auto title = AddGameObject<BlankObject>(UI);
	auto texture = title->AddComponent<Draw2DPolygon>(title);
	texture->LoadTexture(L"Asset/Texture/Title.jpg");
	texture->SetPosition(Vector2(SCREEN_WIDTH_HALF, SCREEN_HEIGHT_HALF));
	texture->SetSize(Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));
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