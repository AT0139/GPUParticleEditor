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

	ImGui::Begin("ParticleSetting", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking);
	{
		//メニューバー
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

		if (ImGui::CollapsingHeader("Spawn"))
		{
			if (ImGui::TreeNode("InitializeParticle"))
			{
				ImGui::SliderInt("Life", &m_currentData->life, 1, 800);
				ImGui::SliderInt("CreateOnceNum", &m_currentData->createOnceNum, 1, 10000);
				ImGui::SliderInt("CreateInterval", &m_currentData->createInterval, 1, 500);

				//サイズ
				if (ImGui::TreeNode("Size"))
				{
					static float size[2] = { 1.0f,1.0f };

					ImGui::SliderFloat2("initial", size, 0.01f, 5.0f);
					if (m_bufferInfo.initialSize.x != size[0] || m_bufferInfo.initialSize.y != size[1])
					{
						m_bufferInfo.initialSize.x = size[0];
						m_bufferInfo.initialSize.y = size[1];
						m_bufferInfo.finalSize.x = size[0];
						m_bufferInfo.finalSize.y = size[1];
						m_currentEmitter->SetInitialSize(m_bufferInfo.initialSize);
						m_currentEmitter->SetFinalSize(m_bufferInfo.finalSize);
					}
					ImGui::TreePop();
				}
				//カラー
				if (ImGui::TreeNode("Color"))
				{
					static float color[4] = { 1.0f,1.0f,1.0f,1.0f };
					ImGui::ColorPicker4("", color);
					m_currentData->color.x = color[0];
					m_currentData->color.y = color[1];
					m_currentData->color.z = color[2];
					m_currentData->color.w = color[3];

					ImGui::TreePop();
				}
				ImGui::TreePop();
			}

			//加速度
			if (ImGui::Checkbox("Add Velocity", &m_flags.addVelocity))
			{
				if (!m_flags.addVelocity)
					m_currentEmitter->SetVelocity(Vector3::Zero);
				else
					m_currentEmitter->SetVelocity(m_bufferInfo.velocity);
			}
			if (m_flags.addVelocity)
			{
				static float vel[3] = { 0.0f,0.0f ,0.0f };
				ImGui::SliderFloat3("Velocity", vel, -5.0f, 5.0f);
				if (m_bufferInfo.velocity.x != vel[0] || m_bufferInfo.velocity.y != vel[1] || m_bufferInfo.velocity.z != vel[1])
				{
					m_bufferInfo.velocity.x = vel[0];
					m_bufferInfo.velocity.y = vel[1];
					m_bufferInfo.velocity.z = vel[2];
					m_currentEmitter->SetVelocity(m_bufferInfo.velocity);
				}
			}
		}

		ImGui::Separator();

		if (ImGui::CollapsingHeader("ParticleUpdate"))
		{
			//サイズ
			ImGui::Checkbox("ScaleSize", &m_flags.scaleSize);
			if (m_flags.scaleSize)
			{
				static float initialSize[2] = { 1.0f,1.0f };
				ImGui::SliderFloat2("initial", initialSize, 0.01f, 5.0f);
				if (m_bufferInfo.initialSize.x != initialSize[0] || m_bufferInfo.initialSize.y != initialSize[1])
				{
					m_bufferInfo.initialSize.x = initialSize[0];
					m_bufferInfo.initialSize.y = initialSize[1];
					m_currentEmitter->SetInitialSize(m_bufferInfo.initialSize);
				}

				static float finalSize[2] = { 1.0f,1.0f };
				ImGui::SliderFloat2("final", finalSize, 0.01f, 5.0f);
				if (m_bufferInfo.finalSize.x != finalSize[0] || m_bufferInfo.finalSize.y != finalSize[1])
				{
					m_bufferInfo.finalSize.x = finalSize[0];
					m_bufferInfo.finalSize.y = finalSize[1];
					m_currentEmitter->SetFinalSize(m_bufferInfo.finalSize);
				}
				ImGui::Spacing();
			}

			//重力
			if (ImGui::Checkbox("Gravity Force", &m_flags.gravity) && !m_flags.gravity)
				m_currentEmitter->SetGravity(Vector3::Zero);
			if (m_flags.gravity)
			{
				static float gravity[3] = {};
				ImGui::SliderFloat3("", gravity, -0.1f, 0.1f);
				Vector3 gravityPower;
				gravityPower.x = gravity[0];
				gravityPower.y = gravity[1];
				gravityPower.z = gravity[2];
				m_currentEmitter->SetGravity(gravityPower);
				ImGui::Spacing();
			}
		}
		//todo : リセット

		static float y = 100.0f;
		static float z = 0.0f;
		ImGui::SliderFloat("y", &y, 0.0f, 250.0f);
		ImGui::SliderFloat("z", &z, 0.0f, 1000.0f);

		m_emitterManager->GetComponent<Transform>()->SetPosition(Vector3(0.0f, y, z));
	}
	ImGui::End();


	if (GET_INPUT.GetKeyTrigger(KEY_CONFIG::RETURN))
	{
		SceneManager::GetInstance().SetScene<Title>();
	}
}