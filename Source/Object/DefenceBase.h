#pragma once

#include "GameObject.h"

class DrawAnimationModel;

namespace MainGame
{
	class DefenceBase : public GameObject
	{
	public:
		DefenceBase();
		~DefenceBase();
		virtual void Update()override;
		virtual void Draw()override;

		virtual void OnTrigger(GameObject* collision) override;

		int GetHP() { return m_hp; }

	private:
		std::shared_ptr<DrawAnimationModel> m_model;
		int m_hp;
	};
};