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
	EmitterInitData m_initData;
	ParticleEmitterManager* m_emitterManager;
};