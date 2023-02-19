#pragma once

#include "Camera.h"

namespace MainGame
{
	class MainCamera : public Camera
	{
	public:
		MainCamera();
		~MainCamera();
		virtual void Update()override;


	private:
		float m_targetYoffset = 2.0f;
		float m_positionYoffset = 0.0f;

		float m_theta;
		float m_delta;

		bool m_isOnField;
		float m_targetYOffset;
		float m_targetYOffsetTemporary;
		float m_lerpRatio;
	};
}