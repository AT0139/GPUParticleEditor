#include "Result.h"
#include "BlankObject.h"
#include "Draw2DPolygon.h"
#include "input.h"
#include "SceneManager.h"
#include "Title.h"

void Result::Init()
{
	auto title = AddGameObject<BlankObject>(UI);
	auto texture = title->AddComponent<Draw2DPolygon>(title);
	texture->LoadTexture(L"Asset/Texture/Result.jpg");
	texture->SetPosition(Vector2(SCREEN_WIDTH_HALF, SCREEN_HEIGHT_HALF));
	texture->SetSize(Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));
}

void Result::Uninit()
{
	Scene::Uninit();
}

void Result::Update()
{
	Scene::Update();

	if (GET_INPUT.GetKeyTrigger(KEY_CONFIG::RETURN))
	{
		SceneManager::GetInstance().SetScene<Title>();
	}
}