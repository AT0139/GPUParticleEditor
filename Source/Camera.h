#pragma once

#include "GameObject.h"

namespace MainGame
{
	class Camera : public GameObject
	{
	public:
		Camera();
		~Camera();
		virtual void Update()override;
		virtual void Draw()override;

		Matrix GetViewMatrix() { return m_viewMatrix; }

	private:
		Vector3 m_target;
		Matrix  m_viewMatrix;

		float m_targetYoffset = 0.0f;
		float m_positionYoffset = 0.0f;
	};
}