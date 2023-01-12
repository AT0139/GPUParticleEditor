#pragma once

#include "Scene.h"
#include "ParticleEmitterManager.h"

class ParticleDemoScene :public Scene
{
public:
	void Init();
	void Uninit();
	void Update();

private:
	std::shared_ptr<ParticleEmitter> m_currentEmitter;
	EmitterInitData* m_currentData;
	ParticleEmitterManager* m_emitterManager;
};