#include "ParticleEmitterManager.h"

ParticleEmitterManager::ParticleEmitterManager()
{
}

void ParticleEmitterManager::Update()
{
	for (auto it : m_particleEmitter) 
		it->SetManagerPosition(GetComponent<Transform>()->GetPosition());

	for (auto it : m_particleEmitter)
		it->Update();
}

void ParticleEmitterManager::Draw()
{
	for (auto it : m_particleEmitter)
		it->Draw(); 
}

std::shared_ptr<ParticleEmitter> ParticleEmitterManager::AddEmitter(EmitterInitData initData)
{
	auto emitter =std::make_shared<ParticleEmitter>(initData);
	m_particleEmitter.push_back(emitter);
	return emitter;
}

int ParticleEmitterManager::GetEmitterIndex(std::shared_ptr<ParticleEmitter> emitter)
{
	auto itr = std::find(m_particleEmitter.begin(), m_particleEmitter.end(), emitter);
	if (itr == m_particleEmitter.end())
		return -1;
	const int index = std::distance(m_particleEmitter.begin(), itr);
	return index;
}
