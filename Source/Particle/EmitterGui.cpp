#include "EmitterGui.h"

namespace
{
	Vector2 SliderVector2(Vector2 initial, float min, float max, const char* label)
	{
		float slider[2] = { initial.x,initial.y };
		ImGui::SliderFloat2(label, slider, min, max);

		return Vector2(slider[0], slider[1]);
	}

	Vector3 SliderVector3(Vector3 initial, float min, float max, const char* label)
	{
		float slider[3] = { initial.x,initial.y,initial.z };
		ImGui::SliderFloat3(label, slider, min, max);

		return Vector3(slider[0], slider[1], slider[2]);
	}

	Vector4 SliderVector4(Vector4 initial, const char* label)
	{
		float slider[4] = { initial.x,initial.y,initial.z,initial.w };
		ImGui::SliderFloat4(label, slider, 0.0f, 1.0f);

		return Vector4(slider[0], slider[1], slider[2], slider[3]);
	}

	Vector4 ColorPickerVector4(Vector4 initial, const char* label)
	{
		float color[4] = { initial.x,initial.y,initial.z,initial.w };
		ImGui::ColorPicker4(label, color, ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_Float);
		return Vector4(color[0], color[1], color[2], color[3]);
	}

	Vector4 ColorEditor(Vector4 color, const char* label)
	{
		Vector4 initialColor = color;
		if (ImGui::TreeNode(label))
		{
			initialColor = SliderVector4(initialColor, "");

			ImVec4 colorVec4(initialColor.x, initialColor.y, initialColor.z, initialColor.w); ImGui::SameLine();
			ImGui::ColorButton("##color", colorVec4);

			//ピッカー
			if (ImGui::Button("OpenColorPicker"))
				ImGui::OpenPopup("ColorPicker", ImGuiPopupFlags_MouseButtonLeft);

			if (ImGui::BeginPopup("ColorPicker"))
			{
				initialColor = ColorPickerVector4(initialColor, "")	;
				ImGui::EndPopup();
			}

			ImGui::TreePop();
		}
		return initialColor;
	}

	Vector2 SizeEditor(Vector2 sz, bool& divisionFlag, const char* label)
	{
		Vector2 size = sz;
		if (ImGui::TreeNode(label))
		{
			ImGui::Spacing(); ImGui::SameLine(0.0f, 50.0f);
			ImGui::Checkbox("division", &divisionFlag);
			if (!divisionFlag)
			{
				ImGui::SliderFloat(label, &size.x, 0.01f, 5.0f);
				size.y = size.x;
			}
			else
			{
				size = SliderVector2(sz, 0.01f, 5.0f, label);
			}
			ImGui::TreePop();
		}
		return size;
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
				//スポーン設定
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
				Vector2 size = SizeEditor(m_bufferInfo.initialSize, m_datas.sizeDivision, "Size");
				if (m_bufferInfo.initialSize != size)
				{
					m_bufferInfo.initialSize = size;
					m_bufferInfo.finalSize = size;
					m_currentEmitter->SetInitialSize(m_bufferInfo.initialSize);
					m_currentEmitter->SetFinalSize(m_bufferInfo.finalSize);
				}

				//カラー
				if (!m_datas.scaleColor)
				{
					auto color = ColorEditor(m_bufferInfo.initialColor, "Color");
					if (color != m_bufferInfo.initialColor)
					{
						m_bufferInfo.initialColor = color;
						m_bufferInfo.finalColor = m_bufferInfo.initialColor;
						m_currentEmitter->SetInitialColor(m_bufferInfo.initialColor);
						m_currentEmitter->SetFinalColor(m_bufferInfo.finalColor);
					}
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

				Vector3 vel = m_bufferInfo.velocity;
				vel = SliderVector3(vel, -5.0f, 5.0f, "Velocity");
				if (m_bufferInfo.velocity != vel|| isSetVelocity)
				{
					m_bufferInfo.velocity = vel;
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
				Vector2 initialSize = SizeEditor(m_bufferInfo.initialSize, m_datas.scaleSizeDivision, "Initial");
				if (m_bufferInfo.initialSize != initialSize)
				{
					m_bufferInfo.initialSize = initialSize;
					m_currentEmitter->SetInitialSize(m_bufferInfo.initialSize);
				}
				Vector2 finalSize = SizeEditor(m_bufferInfo.finalSize, m_datas.scaleSizeDivision, "Final");
				if (m_bufferInfo.finalSize != finalSize)
				{
					m_bufferInfo.finalSize = finalSize;
					m_currentEmitter->SetInitialSize(m_bufferInfo.finalSize);
				}
			}

			//スケールカラー
			ImGui::Checkbox("ScaleColor", &m_datas.scaleColor);
			if (m_datas.scaleColor)
			{
				auto color = ColorEditor(m_bufferInfo.initialColor, "InitialColor");
				if (color != m_bufferInfo.initialColor)
				{
					m_bufferInfo.initialColor = color;
					m_currentEmitter->SetInitialColor(m_bufferInfo.initialColor);
				}

				color = ColorEditor(m_bufferInfo.finalColor, "FinalColor");
				if (color != m_bufferInfo.finalColor)
				{
					m_bufferInfo.finalColor = color;
					m_currentEmitter->SetFinalColor(m_bufferInfo.finalColor);
				}
				ImGui::Spacing();
			}

			//重力
			if (ImGui::Checkbox("Gravity Force", &m_datas.gravity) && !m_datas.gravity)
				m_currentEmitter->SetGravity(Vector3::Zero);
			if (m_datas.gravity)
			{
				Vector3 gravity = m_bufferInfo.gravity;
				gravity = SliderVector3(gravity, -0.1f, 0.1f, "##GravityForce");

				if (gravity != m_bufferInfo.gravity)
				{
					m_bufferInfo.gravity = gravity;
					m_currentEmitter->SetGravity(m_bufferInfo.gravity);
				}
				ImGui::Spacing();
			}
		}
	}
	ImGui::End();
}

