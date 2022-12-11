#pragma once

#include "GameObject.h"

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


		static void ResetBlendRate() { m_blendRate = 0.0f; }
		std::string m_animationName;
	private:
		void Move();

		std::shared_ptr<Rigidbody> m_rigid;

		class AnimationModel* m_model;
		class Audio* m_shotSE;
		class Shadow* m_shadow;

		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11InputLayout* m_vertexLayout;

		const float MOVE_SPEED = 0.1f;
		const float ADD_BLENDRATE = 0.05f;

		int m_frame;
		static float m_blendRate;

		POINTS m_mousePos;
		POINTS m_preMousePos;

		GameObject* m_haveObject;
	};
}