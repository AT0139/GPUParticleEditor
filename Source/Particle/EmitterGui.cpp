#include "EmitterGui.h"

namespace
{
	inline Vector4 SliderVector4(Vector4 initial,const char* label)
	{
		float slider[4] = { initial.x,initial.y,initial.z,initial.w };
		ImGui::SliderFloat4(label, slider, 0.0f, 1.0f);
	
		return Vector4(slider[0], slider[1], slider[2], slider[3]);
	}

	inline Vector4 ColorPickerVector4(Vector4 initial, const char* label)
	{
		float color[4] = { initial.x,initial.y,initial.z,initial.w };
		ImGui::ColorPicker4(label, color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_Float);
		return Vector4(color[0], color[1], color[2], color[3]);
	}
}

EmitterGui::EmitterGui(std::shared_ptr<ParticleEmitter> emitter, std::string emitterName)
{
	m_currentEmitter = emitter;
	m_currentData = m_currentEmitter->GetEmitterData();	
	m_name = emitterName;
	m_datas = {};
}

void EmitterGui::Update()
{
	ImGui::Begin(m_name.c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking);
	{
		if (ImGui::CollapsingHeader("Spawn"))
		{
			if (ImGui::TreeNode("InitializeParticle"))
			{
				if (ImGui::TreeNode("Spawning"))
				{
					bool isSet = false;
					isSet |= ImGui::RadioButton("ParSecond", &m_datas.spawningType, 0); ImGui::SameLine();
					isSet |= ImGui::RadioButton("Burst", &m_datas.spawningType, 1);

					switch (m_datas.spawningType)
					{
					case static_cast<int>(SPAWN_TYPE::PAR_SECOND):
						isSet |= ImGui::SliderFloat("SpawnRate", &m_datas.spawnRate, 1, 2000);
						if (isSet)
						{
							m_currentEmitter->SetSpawnRate(m_datas.spawnRate);
							m_currentEmitter->SetSpawnType(SPAWN_TYPE::PAR_SECOND);
						}
						break;
					case static_cast<int>(SPAWN_TYPE::BURST):
						isSet |= ImGui::SliderInt("CreateOnceNum", &m_datas.onceNum, 1, 5000);
						isSet |= ImGui::SliderFloat("CreateInterval", &m_datas.interval, 1, 500);
						if (isSet)
						{
							m_currentEmitter->SetCreateOnceNum(m_datas.onceNum);
							m_currentEmitter->SetCreateInterval(m_datas.interval);
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
					ImGui::Spacing(); ImGui::SameLine(0.0f, 50.0f);
					ImGui::Checkbox("division", &m_datas.sizeDivision);
					float size[2] = { m_bufferInfo.initialSize.x,m_bufferInfo.initialSize.y };
					if (!m_datas.sizeDivision)
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
					if (!m_datas.scaleColor)
					{
						m_bufferInfo.initialColor = SliderVector4(m_bufferInfo.initialColor, "##Color");
						m_bufferInfo.finalColor = m_bufferInfo.initialColor;

						ImVec4 colorVec4(m_bufferInfo.initialColor.x, m_bufferInfo.initialColor.y,
							m_bufferInfo.initialColor.z, m_bufferInfo.initialColor.w); ImGui::SameLine();
						ImGui::ColorButton("##color", colorVec4);

						m_currentEmitter->SetInitialColor(m_bufferInfo.initialColor);
						m_currentEmitter->SetFinalColor(m_bufferInfo.finalColor);

						//ピッカー
						if (ImGui::Button("OpenColorPicker"))
							ImGui::OpenPopup("ColorPicker", ImGuiPopupFlags_MouseButtonLeft);

						if (ImGui::BeginPopup("ColorPicker"))
						{
							m_bufferInfo.initialColor = ColorPickerVector4(m_bufferInfo.initialColor, "##Color");
							m_bufferInfo.finalColor = m_bufferInfo.initialColor;

							m_currentEmitter->SetInitialColor(m_bufferInfo.initialColor);
							m_currentEmitter->SetFinalColor(m_bufferInfo.finalColor);
							ImGui::EndPopup();
						}
					}
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}

			//加速度
			bool isSetVelocity = false;
			if (ImGui::Checkbox("Add Velocity", &m_datas.addVelocity))
			{

				if (!m_datas.addVelocity)
					m_currentEmitter->SetVelocity(Vector3::Zero, ADD_VELOCITY_TYPE::NONE);
				else
					isSetVelocity = true;
			}
			if (m_datas.addVelocity)
			{
				isSetVelocity |= ImGui::RadioButton("None", &m_datas.addVelocityType, 0); ImGui::SameLine();
				isSetVelocity |= ImGui::RadioButton("InCone", &m_datas.addVelocityType, 1);

				float vel[3] = { m_bufferInfo.velocity.x,m_bufferInfo.velocity.y,m_bufferInfo.velocity.z };
				ImGui::SliderFloat3("Velocity", vel, -5.0f, 5.0f);
				if (m_bufferInfo.velocity.x != vel[0] || m_bufferInfo.velocity.y != vel[1] || m_bufferInfo.velocity.z != vel[2] || isSetVelocity)
				{
					m_bufferInfo.velocity.x = vel[0];
					m_bufferInfo.velocity.y = vel[1];
					m_bufferInfo.velocity.z = vel[2];
					m_currentEmitter->SetVelocity(m_bufferInfo.velocity, static_cast<ADD_VELOCITY_TYPE>(m_datas.addVelocityType));
				}
			}
		}

		ImGui::Separator();

		if (ImGui::CollapsingHeader("ParticleUpdate"))
		{
			//スケールサイズ
			ImGui::Checkbox("ScaleSize", &m_datas.scaleSize);
			if (m_datas.scaleSize)
			{
				ImGui::Spacing(); ImGui::SameLine(0.0f, 50.0f);
				ImGui::Checkbox("division", &m_datas.scaleSizeDivision);
				float initialSize[2] = { m_bufferInfo.initialSize.x,m_bufferInfo.initialSize.y };
				float finalSize[2] = { m_bufferInfo.finalSize.x,m_bufferInfo.finalSize.y };

				if (!m_datas.scaleSizeDivision)
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
			ImGui::Checkbox("ScaleColor", &m_datas.scaleColor);
			if (m_datas.scaleColor)
			{
				if (ImGui::TreeNode("InitialColor"))
				{
					Vector4 initialColor = SliderVector4(m_bufferInfo.initialColor,"Initial");

					ImVec4 colorVec4(initialColor.x, initialColor.y, initialColor.z, initialColor.w); ImGui::SameLine();
					ImGui::ColorButton("##color", colorVec4);
					if (m_bufferInfo.initialColor != initialColor)
					{
						m_bufferInfo.initialColor = initialColor;
						m_currentEmitter->SetInitialColor(m_bufferInfo.initialColor);
					}
					//ピッカー
					if (ImGui::Button("OpenColorPicker"))
						ImGui::OpenPopup("ColorPicker", ImGuiPopupFlags_MouseButtonLeft);

					if (ImGui::BeginPopup("ColorPicker"))
					{
						initialColor = ColorPickerVector4(initialColor, "Initial");
						m_bufferInfo.initialColor = initialColor;
						m_currentEmitter->SetInitialColor(m_bufferInfo.initialColor);
						ImGui::EndPopup();
					}

					ImGui::TreePop();
				}
				if (ImGui::TreeNode("FinalColor"))
				{
					Vector4 finalColor = SliderVector4(m_bufferInfo.finalColor, "Final");
					ImVec4 colorVec4(finalColor.x, finalColor.y, finalColor.z, finalColor.w); ImGui::SameLine();
					ImGui::ColorButton("##color", colorVec4);
					if (m_bufferInfo.finalColor != finalColor)
					{
						m_bufferInfo.finalColor = finalColor;
						m_currentEmitter->SetFinalColor(m_bufferInfo.finalColor);
					}
					//ピッカー
					if (ImGui::Button("OpenColorPicker"))
						ImGui::OpenPopup("ColorPicker", ImGuiPopupFlags_MouseButtonLeft);

					if (ImGui::BeginPopup("ColorPicker"))
					{
						finalColor = ColorPickerVector4(finalColor, "Final");
						m_bufferInfo.finalColor = finalColor;
						m_currentEmitter->SetFinalColor(m_bufferInfo.finalColor);
						ImGui::EndPopup();
					}

					ImGui::TreePop();
				}
				ImGui::Spacing();

			}

			//重力
			if (ImGui::Checkbox("Gravity Force", &m_datas.gravity) && !m_datas.gravity)
				m_currentEmitter->SetGravity(Vector3::Zero);
			if (m_datas.gravity)
			{
				float gravity[3] = { m_bufferInfo.gravity.x ,m_bufferInfo.gravity.y ,m_bufferInfo.gravity.z};
				ImGui::SliderFloat3("##GravityForce", gravity, -0.1f, 0.1f);
				m_bufferInfo.gravity.x = gravity[0];
				m_bufferInfo.gravity.y = gravity[1];
				m_bufferInfo.gravity.z = gravity[2];

				m_currentEmitter->SetGravity(m_bufferInfo.gravity);
				ImGui::Spacing();
			}
		}
	}
	ImGui::End();
}
