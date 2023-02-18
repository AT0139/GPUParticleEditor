#include "Result.h"
#include "Draw2DPolygon.h"
#include "input.h"
#include "SceneManager.h"
#include "Title.h"

void Result::Init()
{
	GET_INPUT.ToggleCursor(true);
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