#include "ParticleDemoScene.h"
#include "Title.h"
#include "Input.h"
#include "SceneManager.h"
#include "SkyDome.h"
#include "ParticleDemoSceneCamera.h"

void ParticleDemoScene::Init()
{
	GET_INPUT.ToggleCursor(true);

	//ÉJÉÅÉâ
	SetCamera<ParticleDemoSceneCamera>();
	AddGameObject<SkyDome>(OBJECT)->GetComponent<Transform>()->SetPosition(Vector3(0.0f, -200.0f, 0.0f));

	m_emitterManager = AddGameObject<ParticleEmitterManager>(EFFECT);
	m_emitterManager->GetComponent<Transform>()->SetPosition(Vector3(0.0f, -40.0f, 0.0f));
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
		if(ImGui::TreeNode("InitData"))
		{
			ImGui::SliderInt("Life", &m_initData.life, 1, 800);
			ImGui::SliderInt("MaxNum", &m_initData.maxNum, 1, 100000);
			ImGui::SliderInt("CreateOnceNum", &m_initData.createOnceNum, 1, 50000);
			ImGui::SliderInt("CreateInterval", &m_initData.createInterval, 1, 1000);
			static float size = 0;
			ImGui::SliderFloat("size", &size, 0.01f, 10.0f);
			m_initData.size = Vector2(size, size);

			ImGui::TreePop();
		}

		if (ImGui::Button("Generate"))
		{
			m_emitterManager->AddEmitter(m_initData);
		}
	}
	ImGui::End();




	if (GET_INPUT.GetKeyTrigger(KEY_CONFIG::RETURN))
	{
		SceneManager::GetInstance().SetScene<Title>();
	}
}