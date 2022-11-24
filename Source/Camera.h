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

		D3DXMATRIX GetViewMatrix() { return m_viewMatrix; }

	private:
		D3DXVECTOR3 m_target;
		D3DXMATRIX  m_viewMatrix;

		float m_targetYoffset = 0.0f;
		float m_positionYoffset = 0.0f;
	};
}