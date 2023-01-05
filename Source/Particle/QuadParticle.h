#pragma once

#include "ParticleBase.h"

class QuadParticle : public ParticleBase
{
public:
	QuadParticle(Vector3 initPosition) : ParticleBase(initPosition) {}
	~QuadParticle() {}

	// ParticleBase を介して継承されました
	virtual Matrix GetWorldMatrix() override;



private:


};