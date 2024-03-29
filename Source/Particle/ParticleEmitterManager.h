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

	void AddParticle(std::string jsonName);
	std::shared_ptr<ParticleEmitter> AddEmitter(EmitterInitData initData);

	int GetEmitterIndex(std::shared_ptr<ParticleEmitter> emitter);
	void DeleteEmitter(std::shared_ptr<ParticleEmitter> emitter);
	void DeleteEmitters();
private:
	std::vector<std::shared_ptr<ParticleEmitter>> m_particleEmitter;
};