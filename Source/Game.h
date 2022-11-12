#pragma once

#include "scene.h"

namespace MainGame
{
	class Game :public Scene
	{
	public:
		void Init();
		void Uninit();
		void Update();
	};
}