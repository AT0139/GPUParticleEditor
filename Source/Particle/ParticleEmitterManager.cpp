#include "ParticleEmitterManager.h"
#include "ParticleSerializeUtility.h"

ParticleEmitterManager::ParticleEmitterManager()
{
}

void ParticleEmitterManager::Update()
{
	for (auto it : m_particleEmitter)
	{
		it->SetScale(GetComponent<Transform>()->GetScale());
		it->SetManagerPosition(GetComponent<Transform>()->GetPosition());
		it->Update();
	}
}

void ParticleEmitterManager::Draw()
{
	for (auto it : m_particleEmitter)
		it->Draw(); 
}

void ParticleEmitterManager::AddParticle(std::string jsonName)
{
	auto inputList = ParticleSerialize::ParticleDeserialize(jsonName);
	for (auto input : inputList)
	{
		AddEmitter(input.data);
	}
}

std::shared_ptr<ParticleEmitter> ParticleEmitterManager::AddEmitter(EmitterInitData initData)
{
	auto emitter = std::make_shared<ParticleEmitter>(initData);
	m_particleEmitter.push_back(emitter);
	return emitter;
}

int ParticleEmitterManager::GetEmitterIndex(std::shared_ptr<ParticleEmitter> emitter)
{
	auto itr = std::find(m_particleEmitter.begin(), m_particleEmitter.end(), emitter);
	if (itr == m_particleEmitter.end())
		return -1;
	const int index = (const int)std::distance(m_particleEmitter.begin(), itr);
	return index;
}

void ParticleEmitterManager::DeleteEmitter(std::shared_ptr<ParticleEmitter> emitter)
{
	int index = GetEmitterIndex(emitter);
	m_particleEmitter.erase(m_particleEmitter.begin() + index);
}

void ParticleEmitterManager::DeleteEmitters()
{
	m_particleEmitter.clear();
}
