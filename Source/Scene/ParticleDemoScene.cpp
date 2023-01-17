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

	ImGui::Begin("ParticleSetting", 0, ImGuiWindowFlags_NoMove|ImGuiWindowFlags_AlwaysAutoResize|ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking);
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
				{

				}
				if (ImGui::MenuItem("Load"))
				{

				}
				ImGui::EndMenu();
			}	
		ImGui::EndMenuBar();
		}

		if (m_currentData)
		{
			if (ImGui::TreeNode("Size"))
			{
				static float size[2] = { 1.0f,1.0f };
				ImGui::SliderFloat2("", size, 0.01f, 5.0f);
				if (m_size.x != size[0] || m_size.y != size[1])
				{
					m_size.x = size[0];
					m_size.y = size[1];
					m_currentEmitter->SetSize(m_size);
				}
				ImGui::TreePop();
			}

			ImGui::SliderInt("Life", &m_currentData->life, 1, 800);
			ImGui::SliderInt("CreateOnceNum", &m_currentData->createOnceNum, 1, 10000);
			ImGui::SliderInt("CreateInterval", &m_currentData->createInterval, 1, 500);

			if (ImGui::TreeNode("Color"))
			{
				static float color[4] = { 1.0f,1.0f,1.0f,1.0f };
				ImGui::SliderFloat4("", color, 0.0f, 1.0f);
				m_currentData->color.x = color[0];
				m_currentData->color.y = color[1];
				m_currentData->color.z = color[2];
				m_currentData->color.w = color[3];

				ImGui::TreePop();
			}

			if (ImGui::TreeNode("Gravity"))
			{
				static float gravity[3] = {};
				ImGui::SliderFloat3("GravityXYZ", gravity, -0.1f, 0.1f);
				Vector3 gravityPower;
				gravityPower.x = gravity[0];
				gravityPower.y = gravity[1];
				gravityPower.z = gravity[2];
				m_currentEmitter->SetGravity(gravityPower);

				ImGui::TreePop();
			}
			//todo : リセット
		}
	}
	ImGui::End();


	if (GET_INPUT.GetKeyTrigger(KEY_CONFIG::RETURN))
	{
		SceneManager::GetInstance().SetScene<Title>();
	}
}