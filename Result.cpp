#include "Result.h"
#include "Polygon2D.h"

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
}
