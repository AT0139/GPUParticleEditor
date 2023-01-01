#pragma once

#include "main.h"
#include "GameObject.h"

namespace MainGame
{
	class Bullet :public GameObject
	{
	public:
		Bullet();
		~Bullet();
		virtual void Update()override;
		virtual void Draw()override {}

		void SetVelocity(Vector3 velocity);
	private:
		const float MOVE_SPEED = 0.3f;
		const int DESTROY_COUNT = 60;

		Vector3 m_forward;

		int m_count;
	};
}