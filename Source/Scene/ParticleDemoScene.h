#pragma once

#include "Scene.h"
#include "ParticleEmitterManager.h"
#include "EmitterGui.h"

class ParticleDemoScene :public Scene
{
public:
	void Init();
	void Uninit();
	void Update();

private:
	void AddEmitter();

	ParticleEmitterManager* m_emitterManager;
	std::list<std::shared_ptr<EmitterGui>> m_emitterList;
};