#pragma once

#include "GameObject.h"

namespace MainGame
{
	class Explosion : public GameObject
	{
	public:
		Explosion();
		~Explosion();
		virtual void Update()override;
		virtual void Draw()override;

	private:
	};
}