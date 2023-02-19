#pragma once

#include "scene.h"
#include "CollisionManager.h"

class CollisionManager;

namespace MainGame
{
	class Game :public Scene
	{
	public:
		void Init();
		void Uninit();
		void Update();

	private:
		std::unique_ptr<CollisionManager> m_collisionManager;

		class DefenceBase* m_defenceBase;
	};
}