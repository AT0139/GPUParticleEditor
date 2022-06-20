#pragma once

#include "scene.h"

class Game :public Scene
{
public:
	void Init();
	void Uninit();
	void Update();
};