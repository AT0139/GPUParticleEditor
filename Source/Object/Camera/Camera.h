#pragma once

#include "GameObject.h"

class Camera : public GameObject
{
public:
	Camera();
	virtual ~Camera();
	virtual void Update()override;
	virtual void Draw()override;

	virtual Matrix GetViewMatrix() { return m_viewMatrix; }
	virtual Matrix GetProjectionMatrix() { return m_projection; }
	virtual Vector3 GetCamaraForward();
	virtual Vector3 GetCamaraRight();

private:
	Vector3 m_target;
	Vector3 m_cameraPos;
	Matrix  m_viewMatrix;
	Matrix  m_projection;

	float m_targetYoffset = 2.0f;
	float m_positionYoffset = 0.0f;

	float m_theta;
	float m_delta;

	bool m_isOnField;
	float m_targetYOffset;
	float m_targetYOffsetTemporary;
	float m_lerpRatio;
};