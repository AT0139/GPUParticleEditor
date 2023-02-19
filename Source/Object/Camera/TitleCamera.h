#pragma once

#include "Camera.h"

class TitleCamera : public Camera
{
public:
	TitleCamera();
	virtual ~TitleCamera();
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