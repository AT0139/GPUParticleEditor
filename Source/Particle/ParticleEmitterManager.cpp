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

void ParticleEmitterManager::AddEmitter(EmitterInitData initData)
{
	m_particleEmitter.push_back(std::make_shared<ParticleEmitter>(initData));
}
