﻿#pragma once

#include "GameObject.h"
#include "ParticleEmitter.h"

class ParticleEmitterManager : public GameObject
{
public:
	ParticleEmitterManager();
	~ParticleEmitterManager() {}

	// GameObject を介して継承されました
	virtual void Update() override;
	virtual void Draw() override;

	std::shared_ptr<ParticleEmitter> AddEmitter(EmitterInitData initData);
private:
	std::list<std::shared_ptr<ParticleEmitter>> m_particleEmitter;
};