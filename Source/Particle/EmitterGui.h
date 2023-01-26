#pragma once

#include "ParticleEmitter.h"


class EmitterGui
{
public:
	EmitterGui(std::shared_ptr<ParticleEmitter> emitter, std::string emitterName);
	~EmitterGui() {}

	void Update();

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

	};


	std::shared_ptr<ParticleEmitter> m_currentEmitter;
	EmitterInitData* m_currentData;
	BufferInfo m_bufferInfo;
	EmitterDatas m_datas;

	std::string m_name;
};