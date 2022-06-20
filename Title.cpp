#include "Title.h"
#include "Polygon2D.h"

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
}
