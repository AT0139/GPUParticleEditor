#include "ParticleDemoScene.h"
#include "Title.h"
#include "Input.h"
#include "SceneManager.h"
#include "SkyDome.h"
#include "ParticleDemoSceneCamera.h"

void ParticleDemoScene::Init()
{
	GET_INPUT.ToggleCursor(true);

	//カメラ
	SetCamera<ParticleDemoSceneCamera>();
	AddGameObject<SkyDome>(OBJECT)->GetComponent<Transform>()->SetPosition(Vector3(0.0f, -200.0f, 0.0f));

	m_emitterManager = AddGameObject<ParticleEmitterManager>(EFFECT);
	m_emitterManager->GetComponent<Transform>()->SetPosition(Vector3(0.0f, 100.0f, 0.0f));


	EmitterInitData initData = {};
	m_currentEmitter = m_emitterManager->AddEmitter(initData);
	m_currentData = m_currentEmitter->GetEmitterData();
}

void ParticleDemoScene::Uninit()
{
	Scene::Uninit();
}

void ParticleDemoScene::Update()
{
	Scene::Update();

	ImGui::Begin("ParticleSetting", 0, ImGuiWindowFlags_NoMove|ImGuiWindowFlags_AlwaysAutoResize| ImGuiWindowFlags_NoDocking);
	{
		if (m_currentData)
		{
			static float size = 1.0f;
			ImGui::SliderFloat("size", &size, 0.01f, 10.0f);
			m_currentEmitter->SetSize(Vector2(size, size));

			ImGui::SliderInt("Life", &m_currentData->life, 1, 800);
			ImGui::SliderInt("CreateOnceNum", &m_currentData->createOnceNum, 1, 50000);
			ImGui::SliderInt("CreateInterval", &m_currentData->createInterval, 1, 1000);

			static float color[3] = {1.0f,1.0f,1.0f};
			ImGui::SliderFloat3("color", color, 0.0f, 1.0f);
			m_currentData->color.x = color[0];
			m_currentData->color.y = color[1];
			m_currentData->color.z = color[2];

			//todo : リセット
		}
	}
	ImGui::End();


	if (GET_INPUT.GetKeyTrigger(KEY_CONFIG::RETURN))
	{
		SceneManager::GetInstance().SetScene<Title>();
	}
}