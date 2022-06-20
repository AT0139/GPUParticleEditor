#include "Result.h"
#include "Polygon2D.h"
#include "input.h"
#include "manager.h"
#include "Title.h"

void Result::Init()
{
	AddGameObject<Polygon2D>(UI);
}

void Result::Uninit()
{
	Scene::Uninit();
}

void Result::Update()
{
	Scene::Update();

	if (Input::GetKeyTrigger(VK_RETURN))
	{
		Manager::SetScene<Title>();
	}
}
