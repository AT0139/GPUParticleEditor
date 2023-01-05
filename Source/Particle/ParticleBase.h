#pragma once

class ParticleBase
{
public:
	ParticleBase(Vector3 initPosition) : m_position(initPosition){}
	virtual ~ParticleBase() {}

	virtual Matrix GetWorldMatrix() { return Matrix(); }

protected:

	Vector3 m_scale;
	Vector3 m_position;
	Vector3 m_rotation;
};