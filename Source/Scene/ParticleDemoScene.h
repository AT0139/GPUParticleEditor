#pragma once

#include "Scene.h"
#include "ParticleEmitterManager.h"
#include "EmitterGui.h"
#include "ParticleSerializeUtility.h"



class ParticleDemoScene :public Scene
{
public:
	void Init();
	void Uninit();
	void Update();

private:
	void AddEmitter();
	void AddEmitter(EmitterInitData initData, std::string emitterName);

	void Save();
	void Load();


	void InitDeserialize();

	std::vector<std::string> m_savedParticles;	//保存してあるパーティクル名リスト
	bool m_isSaving;
	bool m_isLoading;
	ParticleEmitterManager* m_emitterManager;
	std::vector<std::shared_ptr<EmitterGui>> m_emitterList;
};