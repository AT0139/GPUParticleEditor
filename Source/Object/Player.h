#pragma once

#include "GameObject.h"

class DrawAnimationModel;

namespace MainGame
{
	class Player :public GameObject
	{
	public:
		Player();
		~Player();
		virtual void Update()override;
		virtual void Draw()override;
		void OnCollision(GameObject* collision)override;
		void OnTrigger(GameObject* collision)override;

	private:
		void Move();
		void Jump();

		void Throw();

		std::shared_ptr<Rigidbody> m_rigid;
		std::shared_ptr<Transform> m_transform;

		//std::shared_ptr<DrawAnimationModel> m_model;

		class Audio* m_shotSE;
		class Shadow* m_shadow;


		const float MOVE_SPEED;
		const float JUMP_FORCE;
		const float THROW_FORCE;

		GameObject* m_haveObject;
	};
}