#include "ParticleEmitterManager.h"

ParticleEmitterManager::ParticleEmitterManager()
{
	EmitterInitData initData = {};
	initData.size = Vector2(1.0f, 1.0f);
	initData.direction = Vector3::Up * 0.0001f;
	initData.life = 300;
	initData.filePath = L"Asset\\Texture\\BlueBloom.png";
	initData.maxNum = 100000;
	initData.createInterval = 10;
	initData.createOnceNum = 50;
	m_particleEmitter.push_back(std::make_shared<ParticleEmitter>(initData));

	initData.size = Vector2(0.3f, 0.3f);
	initData.direction = -Vector3::Up * 0.001f;
	initData.life = 500;
	initData.filePath = L"Asset\\Texture\\WhiteBloom.png";
	initData.maxNum = 100000;
	initData.createInterval = 50;
	initData.createOnceNum = 5000;
	m_particleEmitter.push_back(std::make_shared<ParticleEmitter>(initData));
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
