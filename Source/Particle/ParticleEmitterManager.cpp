#include "ParticleEmitterManager.h"

ParticleEmitterManager::ParticleEmitterManager()
{
	EmitterInitData initData = {};
	initData.size = Vector2(1.0f, 1.0f);
	initData.direction = Vector3::Up;
	m_particleEmitter = std::make_shared<ParticleEmitter>(initData);
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
