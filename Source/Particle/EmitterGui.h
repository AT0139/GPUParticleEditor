#pragma once

#include "ParticleEmitter.h"


class EmitterGui
{
public:
	EmitterGui(std::shared_ptr<ParticleEmitter> emitter, std::string emitterName);
	~EmitterGui() {}

	void Update();

	std::shared_ptr<ParticleEmitter> GetEmitter() { return m_currentEmitter; }
	std::string GetEmitterName() { return m_name; }

private:
	struct EmitterDatas
	{
		int spawningType;
		float spawnRate		= 1;
		float interval		= 1;
		int onceNum			= 1;
		bool sizeDivision;
		bool scaleSizeDivision;
		bool addVelocity;
		int addVelocityType;
		bool scaleSize;
		bool scaleColor;
		bool gravity;
		int maxLife			= 10;
		Vector3 spawnPos;
	};

	//todo ; ParFrame,ParUnit,スポーン確立(間引き)
	enum class SPAWN_TYPE
	{
		PAR_SECOND,
		//PAR_FRAME,
		BURST
		//PAR_UNIT
	};

	std::shared_ptr<ParticleEmitter> m_currentEmitter;
	ParticleInfo m_bufferInfo;
	EmitterDatas m_datas;

	std::string m_name;
	std::string m_tempName;
};