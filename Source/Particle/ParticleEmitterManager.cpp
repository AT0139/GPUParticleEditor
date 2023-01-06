#include "ParticleEmitterManager.h"

ParticleEmitterManager::ParticleEmitterManager()
{
	m_particleEmitter = std::make_shared<ParticleEmitter>();
	m_particleEmitter->SetManagerPosition(GetComponent<Transform>()->GetPosition());
	m_particleEmitter->Load(L"Asset\\Texture\\Bullet.png");
}

void ParticleEmitterManager::Update()
{
	m_particleEmitter->Update();
}

void ParticleEmitterManager::Draw()
{
	m_particleEmitter->Draw();
}
