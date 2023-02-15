#include "EmitterGui.h"
#include "ImGuiUtility.h"

EmitterGui::EmitterGui(std::shared_ptr<ParticleEmitter> emitter, std::string emitterName)
{
	m_currentEmitter = emitter;
	m_name = emitterName;
	m_tempName = emitterName;
	m_datas = {};
}

void EmitterGui::Update()
{
	ImGui::Begin(m_name.c_str(), 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDocking);
	{
		auto name = m_tempName.c_str();
		ImGui::InputText("EmitterName", (char*)name, 30); ImGui::SameLine();
		if (ImGui::Button("Apply"))
			m_name = m_tempName.c_str();

		if (ImGui::CollapsingHeader("Spawn"))
		{
			//�X�|�[���ݒ�
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
					}
					break;
				case static_cast<int>(SPAWN_TYPE::BURST):
					isSet |= ImGui::SliderInt("CreateOnceNum", &m_datas.onceNum, 1, 5000);
					isSet |= ImGui::SliderFloat("CreateInterval", &m_datas.interval, 1, 500);
					if (isSet)
					{
						m_currentEmitter->SetCreateOnceNum(m_datas.onceNum);
						m_currentEmitter->SetCreateInterval(m_datas.interval);
					}
					break;
				default:
					break;
				}

				ImGui::SliderInt("Life", &m_datas.maxLife, 1, 500);
				if (m_datas.maxLife != m_bufferInfo.maxLife)
				{
					m_bufferInfo.maxLife = m_datas.maxLife;
					m_currentEmitter->SetLife(m_bufferInfo.maxLife);
				}
				ImGui::TreePop();
			}
			//�T�C�Y
			Vector2 size = SizeEditor(m_bufferInfo.initialSize, m_datas.sizeDivision, "Size");
			if (m_bufferInfo.initialSize != size)
			{
				m_bufferInfo.initialSize = size;
				m_bufferInfo.finalSize = size;
				m_currentEmitter->SetInitialSize(m_bufferInfo.initialSize);
				m_currentEmitter->SetFinalSize(m_bufferInfo.finalSize);
			}

			//�J���[
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

			//�X�|�[���ʒu
			if (ImGui::TreeNode("SpawnPosition"))
			{
				Vector3 pos = SliderVector3(m_datas.spawnPos, -50, 50, "##SpawnPosition");
				if (pos != m_datas.spawnPos)
				{
					m_datas.spawnPos = pos;
					m_currentEmitter->SetOffsetPosition(pos);
				}
				ImGui::TreePop();
			}

			//�����x
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
				if (m_bufferInfo.velocity != vel || isSetVelocity)
				{
					m_bufferInfo.velocity = vel;
					m_currentEmitter->SetVelocity(m_bufferInfo.velocity, static_cast<ADD_VELOCITY_TYPE>(m_datas.addVelocityType));
				}
			}
		}

		ImGui::Separator();

		if (ImGui::CollapsingHeader("ParticleUpdate"))
		{
			//�X�P�[���T�C�Y
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
					m_currentEmitter->SetFinalSize(m_bufferInfo.finalSize);
				}
			}

			//�X�P�[���J���[
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

			//�d��
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

