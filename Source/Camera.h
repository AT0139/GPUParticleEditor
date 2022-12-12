﻿#pragma once

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
		Vector3 GetCamaraForward();

	private:
		Vector3 m_target;
		Vector3 m_cameraPos;
		Matrix  m_viewMatrix;

		float m_targetYoffset = 2.0f;
		float m_positionYoffset = 0.0f;

		POINTS m_mousePos;
		POINTS m_preMousePos;

		float m_theta;
		float m_delta;
	};
}