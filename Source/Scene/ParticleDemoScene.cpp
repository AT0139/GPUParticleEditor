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

	ImGui::Begin("ParticleSetting", 0,ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking);
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
				if (ImGui::TreeNode("Spawning"))
				{
					bool isSet = false;
					static int type = 0;
					isSet |= ImGui::RadioButton("ParSecond", &type, 0); ImGui::SameLine();
					isSet |= ImGui::RadioButton("Burst", &type, 1);
					static float spawnRate = 1;
					static float interval = 1;
					static int onceNum = 1;
					switch (type)
					{
					case static_cast<int>(SPAWN_TYPE::PAR_SECOND):
						isSet |= ImGui::SliderFloat("SpawnRate", &spawnRate, 1, 2000);
						if (isSet)
						{
							m_currentEmitter->SetSpawnRate(spawnRate);
							m_currentEmitter->SetSpawnType(SPAWN_TYPE::PAR_SECOND);
						}
						break;
					case static_cast<int>(SPAWN_TYPE::BURST):
						isSet |= ImGui::SliderInt("CreateOnceNum", &onceNum, 1, 5000);
						isSet |= ImGui::SliderFloat("CreateInterval", &interval, 1, 500);
						if (isSet)
						{
							m_currentEmitter->SetCreateOnceNum(onceNum);
							m_currentEmitter->SetCreateInterval(interval);
							m_currentEmitter->SetSpawnType(SPAWN_TYPE::BURST);
						}
						break;
					default:
						break;
					}
					ImGui::SliderInt("Life", &m_currentData->life, 1, 800);
					ImGui::TreePop();
				}
				//サイズ
				if (ImGui::TreeNode("Size"))
				{
					static bool division = false;

					ImGui::Spacing(); ImGui::SameLine(0.0f, 50.0f);
					ImGui::Checkbox("division", &division);
					static float size[2] = { 1.0f,1.0f };
					if (!division)
					{
						ImGui::SliderFloat("size", &size[0], 0.01f, 5.0f);
						size[1] = size[0];
					}
					else
					{
						ImGui::SliderFloat2("size", size, 0.01f, 5.0f);
					}
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
					ImGui::SliderFloat4("##Color", color, 0.0f, 1.0f);
					ImVec4 colorVec4(color[0], color[1], color[2], color[3]); ImGui::SameLine();
					ImGui::ColorButton("##color", colorVec4);
					m_bufferInfo.initialColor.x = color[0];
					m_bufferInfo.initialColor.y = color[1];
					m_bufferInfo.initialColor.z = color[2];
					m_bufferInfo.initialColor.w = color[3];
					m_bufferInfo.finalColor.x = color[0];
					m_bufferInfo.finalColor.y = color[1];
					m_bufferInfo.finalColor.z = color[2];
					m_bufferInfo.finalColor.w = color[3];
					m_currentEmitter->SetInitialColor(m_bufferInfo.initialColor);
					m_currentEmitter->SetFinalColor(m_bufferInfo.finalColor);

					//ピッカー
					if (ImGui::Button("OpenColorPicker"))
						ImGui::OpenPopup("ColorPicker", ImGuiPopupFlags_MouseButtonLeft);

					if(ImGui::BeginPopup("ColorPicker"))
					{
						ImGui::ColorPicker4("", color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_Float);
						m_bufferInfo.initialColor.x = color[0];
						m_bufferInfo.initialColor.y = color[1];
						m_bufferInfo.initialColor.z = color[2];
						m_bufferInfo.initialColor.w = color[3];
						m_bufferInfo.finalColor.x = color[0];
						m_bufferInfo.finalColor.y = color[1];
						m_bufferInfo.finalColor.z = color[2];
						m_bufferInfo.finalColor.w = color[3];
						m_currentEmitter->SetInitialColor(m_bufferInfo.initialColor);
						m_currentEmitter->SetFinalColor(m_bufferInfo.finalColor);
						ImGui::EndPopup();
					}

					ImGui::TreePop();
				}
				ImGui::TreePop();
			}

			//加速度
			bool isSetVelocity = false;
			if (ImGui::Checkbox("Add Velocity", &m_flags.addVelocity))
			{
				
				if (!m_flags.addVelocity)
					m_currentEmitter->SetVelocity(Vector3::Zero, ADD_VELOCITY_TYPE::NONE);
				else
					isSetVelocity = true;
			}
			if (m_flags.addVelocity)
			{
				static int type = 0;
				isSetVelocity |= ImGui::RadioButton("None", &type, 0); ImGui::SameLine();
				isSetVelocity |= ImGui::RadioButton("InCone", &type, 1);

				static float vel[3] = { 0.0f,0.0f ,0.0f };
				ImGui::SliderFloat3("Velocity", vel, -5.0f, 5.0f);
				if (m_bufferInfo.velocity.x != vel[0] || m_bufferInfo.velocity.y != vel[1] || m_bufferInfo.velocity.z != vel[2]|| isSetVelocity)
				{
					m_bufferInfo.velocity.x = vel[0];
					m_bufferInfo.velocity.y = vel[1];
					m_bufferInfo.velocity.z = vel[2];
					m_currentEmitter->SetVelocity(m_bufferInfo.velocity, static_cast<ADD_VELOCITY_TYPE>(type));
				}
			}
		}

		ImGui::Separator();

		if (ImGui::CollapsingHeader("ParticleUpdate"))
		{
			//スケールサイズ
			ImGui::Checkbox("ScaleSize", &m_flags.scaleSize);
			if (m_flags.scaleSize)
			{
				static bool division = false;
				ImGui::Spacing(); ImGui::SameLine(0.0f,50.0f);
				ImGui::Checkbox("division", &division);
				static float initialSize[2] = { 1.0f,1.0f };
				static float finalSize[2] = { 1.0f,1.0f };

				if (!division)
				{
					ImGui::SliderFloat("initial", &initialSize[0], 0.01f, 5.0f);
					ImGui::SliderFloat("final", &finalSize[0], 0.01f, 5.0f);
					initialSize[1] = initialSize[0];
					finalSize[1] = finalSize[0];
				}
				else
				{
					ImGui::SliderFloat2("initial", initialSize, 0.01f, 5.0f);
					ImGui::SliderFloat2("final", finalSize, 0.01f, 5.0f);
				}
				if (m_bufferInfo.initialSize.x != initialSize[0] || m_bufferInfo.initialSize.y != initialSize[1])
				{
					m_bufferInfo.initialSize.x = initialSize[0];
					m_bufferInfo.initialSize.y = initialSize[1];
					m_currentEmitter->SetInitialSize(m_bufferInfo.initialSize);
				}

				if (m_bufferInfo.finalSize.x != finalSize[0] || m_bufferInfo.finalSize.y != finalSize[1])
				{
					m_bufferInfo.finalSize.x = finalSize[0];
					m_bufferInfo.finalSize.y = finalSize[1];
					m_currentEmitter->SetFinalSize(m_bufferInfo.finalSize);
				}
				ImGui::Spacing();
			}

			//スケールカラー
			ImGui::Checkbox("ScaleColor", &m_flags.scaleColor);
			if (m_flags.scaleColor)
			{
				if (ImGui::TreeNode("InitialColor"))
				{
					static float initialColor[4] = { 1.0f,1.0f,1.0f,1.0f };
					ImGui::SliderFloat4("initial", initialColor, 0.0f, 1.0f);
					ImVec4 colorVec4(initialColor[0], initialColor[1], initialColor[2], initialColor[3]); ImGui::SameLine();
					ImGui::ColorButton("##color", colorVec4);
					if (m_bufferInfo.initialColor.x != initialColor[0] || m_bufferInfo.initialColor.y != initialColor[1] ||
						m_bufferInfo.initialColor.z != initialColor[2] || m_bufferInfo.initialColor.w != initialColor[3])
					{
						m_bufferInfo.initialColor.x = initialColor[0];
						m_bufferInfo.initialColor.y = initialColor[1];
						m_bufferInfo.initialColor.z = initialColor[2];
						m_bufferInfo.initialColor.w = initialColor[3];
						m_currentEmitter->SetInitialColor(m_bufferInfo.initialColor);
					}
					//ピッカー
					if (ImGui::Button("OpenColorPicker"))
						ImGui::OpenPopup("ColorPicker", ImGuiPopupFlags_MouseButtonLeft);

					if (ImGui::BeginPopup("ColorPicker"))
					{
						ImGui::ColorPicker4("##InitialColor", initialColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_Float);
						m_bufferInfo.initialColor.x = initialColor[0];
						m_bufferInfo.initialColor.y = initialColor[1];
						m_bufferInfo.initialColor.z = initialColor[2];
						m_bufferInfo.initialColor.w = initialColor[3];
						m_currentEmitter->SetInitialColor(m_bufferInfo.initialColor);
						ImGui::EndPopup();
					}

					ImGui::TreePop();
				}
				if (ImGui::TreeNode("FinalColor"))
				{
					static float finalColor[4] = { 1.0f,1.0f,1.0f,1.0f };
					ImGui::SliderFloat4("final", finalColor, 0.0f,1.0f);
					ImVec4 colorVec4(finalColor[0], finalColor[1], finalColor[2], finalColor[3]); ImGui::SameLine();
					ImGui::ColorButton("##color", colorVec4);
					if (m_bufferInfo.finalColor.x != finalColor[0] || m_bufferInfo.initialColor.y != finalColor[1] ||
						m_bufferInfo.finalColor.z != finalColor[2] || m_bufferInfo.initialColor.w != finalColor[3])
					{
						m_bufferInfo.finalColor.x = finalColor[0];
						m_bufferInfo.finalColor.y = finalColor[1];
						m_bufferInfo.finalColor.z = finalColor[2];
						m_bufferInfo.finalColor.w = finalColor[3];
						m_currentEmitter->SetFinalColor(m_bufferInfo.finalColor);
					}
					//ピッカー
					if (ImGui::Button("OpenColorPicker"))
						ImGui::OpenPopup("ColorPicker", ImGuiPopupFlags_MouseButtonLeft);

					if (ImGui::BeginPopup("ColorPicker"))
					{
						ImGui::ColorPicker4("##FinalColor", finalColor, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_Float);
						m_bufferInfo.finalColor.x = finalColor[0];
						m_bufferInfo.finalColor.y = finalColor[1];
						m_bufferInfo.finalColor.z = finalColor[2];
						m_bufferInfo.finalColor.w = finalColor[3];
						m_currentEmitter->SetFinalColor(m_bufferInfo.finalColor);
						ImGui::EndPopup();
					}

					ImGui::TreePop();
				}
				ImGui::Spacing();

			}

			//重力
			if (ImGui::Checkbox("Gravity Force", &m_flags.gravity) && !m_flags.gravity)
				m_currentEmitter->SetGravity(Vector3::Zero);
			if (m_flags.gravity)
			{
				static float gravity[3] = {};
				ImGui::SliderFloat3("##GravityForce", gravity, -0.1f, 0.1f);
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