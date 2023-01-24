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
	struct EmitterFlags
	{
		bool addVelocity;
		bool scaleSize;
		bool scaleColor;
		bool gravity;
	};

	std::shared_ptr<ParticleEmitter> m_currentEmitter;
	EmitterInitData* m_currentData;
	ParticleEmitterManager* m_emitterManager;
	BufferInfo m_bufferInfo;
	EmitterFlags m_flags;
};