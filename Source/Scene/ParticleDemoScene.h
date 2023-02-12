#pragma once

#include "Scene.h"
#include "ParticleEmitterManager.h"
#include "EmitterGui.h"

struct ParticleSerializeData
{
	template<class Archive>
	void serialize(Archive& archive)
	{
	}
};

class ParticleDemoScene :public Scene
{
public:
	void Init();
	void Uninit();
	void Update();

private:
	void AddEmitter();
	void Serialize();

	ParticleEmitterManager* m_emitterManager;
	std::list<std::shared_ptr<EmitterGui>> m_emitterList;
};