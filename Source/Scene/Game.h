#pragma once

#include "scene.h"
#include "CollisionManager.h"
#include "SerializeManager.h"

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
		std::unique_ptr<SerializeManager> m_serializeManger;

		class DefenceBase* m_defenceBase;
	};
}