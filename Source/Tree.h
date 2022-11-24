#pragma once

#include "GameObject.h"

namespace MainGame
{
	class Tree : public GameObject
	{
	public:
		Tree();
		~Tree();
		virtual void Update()override;
		virtual void Draw()override;

	private:
	};
}