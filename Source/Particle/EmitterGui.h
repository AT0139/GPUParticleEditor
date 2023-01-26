#pragma once

#include "ParticleEmitter.h"


class EmitterGui
{
public:
	EmitterGui(std::shared_ptr<ParticleEmitter> emitter, std::string emitterName);
	~EmitterGui() {}

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
	BufferInfo m_bufferInfo;
	EmitterFlags m_flags;

	std::string m_name;
};