#pragma once

#include "Scene.h"
#include "ParticleEmitterManager.h"
#include "EmitterGui.h"
#include "ParticleSerializeUtility.h"



struct ParticleSerializeData
{
	std::string emitterName;
	EmitterInitData data;

	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(CEREAL_NVP(emitterName),CEREAL_NVP(data.filePath),
			CEREAL_NVP(data.gravity), CEREAL_NVP(data.maxLife),
			CEREAL_NVP(data.velocity), CEREAL_NVP(data.velocityType), CEREAL_NVP(data.initialSize), CEREAL_NVP(data.finalSize),
			CEREAL_NVP(data.initialColor), CEREAL_NVP(data.finalColor), CEREAL_NVP(data.offsetPosition));
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
	void AddEmitter(EmitterInitData initData, std::string emitterName);

	void ToSerialize(std::string particleName);
	void ToDeserialize();

	template<class Archive>
	void serialize(Archive& archive, std::list<ParticleSerializeData>& particles)
	{
		archive(CEREAL_NVP(particles));
	}

	template<class Archive>
	void serialize(Archive& archive, std::list<std::string>& particles)
	{
		archive(CEREAL_NVP(particles));
	}

	std::list<std::string> m_savedParticles;
	bool m_isSaving;
	ParticleEmitterManager* m_emitterManager;
	std::list<std::shared_ptr<EmitterGui>> m_emitterList;
};