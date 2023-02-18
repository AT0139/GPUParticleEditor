#include "ParticleDemoScene.h"
#include "Title.h"
#include "Input.h"
#include "SceneManager.h"
#include "SkyDome.h"
#include "ParticleDemoSceneCamera.h"
#include "ImGuiUtility.h"
#include "Field.h"
#include "Polygon2D.h"

namespace
{
	//ListBox用関数
	bool StringGetter(void* data, int index, const char** output)
	{
		std::string* string = (std::string*)data;
		std::string& current_waypoint = string[index];

		*output = current_waypoint.c_str(); // not very safe

		return true;
	}
}

void ParticleDemoScene::Init()
{
	GET_INPUT.ToggleCursor(true);

	//カメラ
	SetCamera<ParticleDemoSceneCamera>();
	AddGameObject<SkyDome>(OBJECT)->GetComponent<Transform>()->SetPosition(Vector3(0.0f, -00.0f, 0.0f));

	m_emitterManager = AddGameObject<ParticleEmitterManager>(EFFECT);

	InitDeserialize();

	//auto field = AddGameObject<Field>(OBJECT);
	//field->GetComponent<Transform>()->SetPosition(Vector3(0.0f, -10.0f, 0.0f));
	//AddDepthObject(field);

	//AddGameObject<Polygon2D>(UI);
}

void ParticleDemoScene::Uninit()
{
	Scene::Uninit();
}

void ParticleDemoScene::Update()
{
	Scene::Update();

	static int emitterListCurrent = 0;
	//メインウィンドウ
	ImGui::Begin("ParticleSetting", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking);
	{
		if(ImGui::TreeNode("Render"))
		{
			LARGE_INTEGER qpf;
			LARGE_INTEGER qpc;

			QueryPerformanceFrequency(&qpf);
			QueryPerformanceCounter(&qpc);
			LONGLONG time = qpc.QuadPart;
			static LONGLONG lastTime;

			float drawTime = (time - lastTime) * 1000.0f / qpf.QuadPart;
			lastTime = time;

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			static float value[180];
			for (int i = 0; i < 179; i++)
				value[i] = value[i + 1];
			value[170] = drawTime;
			ImGui::PlotLines("", value, sizeof(value) / sizeof(float), 0, NULL, 0.0f, 100.0f, ImVec2(0, 50));
			ImGui::TreePop();
		}
		//メニューバー
		if (ImGui::BeginMenuBar())
		{
			//ファイルメニュー
			if (ImGui::BeginMenu("File"))
			{
				//セーブ(シリアライズ)
				if (ImGui::MenuItem("Save"))
				{
					m_isSaving = true;
				}
				//ロード(デシリアライズ)
				if (ImGui::MenuItem("Load"))
				{
					m_isLoading = true;
					
				}
				if (ImGui::MenuItem("Close"))
				{
					m_emitterList.clear();
					m_emitterManager->DeleteEmitters();
					
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		//セーブ中じゃない場合操作可
		if (!m_isSaving && !m_isLoading)
		{
			//エミッタリストボックス
			std::vector<std::string> emitterNameList;
			for (auto it : m_emitterList)
				emitterNameList.push_back(it->GetEmitterName());
			ImGui::ListBox("##Particles", &emitterListCurrent, StringGetter,
				emitterNameList.data(), (int)emitterNameList.size());

			//エミッタの追加
			if (ImGui::Button("AddEmitter"))
			{
				AddEmitter();
			}ImGui::SameLine();
			if (ImGui::Button("DeleteEmitter"))
			{
				m_emitterManager->DeleteEmitter(m_emitterList[emitterListCurrent]->GetEmitter());
				m_emitterList.erase(m_emitterList.begin() + emitterListCurrent);
				emitterListCurrent = 0;
			}

			static Vector2 particlePos;
			particlePos = SliderVector2(particlePos, -50.0f, 50.0f, "ParticlePosition");
			m_emitterManager->GetComponent<Transform>()->SetPosition(Vector3(0.0f, particlePos.x, particlePos.y));
		}


	}
	ImGui::End();

	//セーブ処理中
	if (m_isSaving)
		Save();
	//ロード処理中
	else if (m_isLoading)
		Load();
	else
	{
		for (auto it : m_emitterList)
		{
			if (it == m_emitterList[emitterListCurrent])
			{
				ImGui::PushID(&it);
				it->Update();
				ImGui::PopID();
			}
		}

		//if (GET_INPUT.GetKeyTrigger(KEY_CONFIG::RETURN))
		//{
		//	SceneManager::GetInstance().SetScene<Title>();
		//}
	}
}

void ParticleDemoScene::AddEmitter()
{
	EmitterInitData initData = {};
	auto emitter = m_emitterManager->AddEmitter(initData);
	int index = m_emitterManager->GetEmitterIndex(emitter);
	if (index == -1)
		assert(nullptr);

	std::string sequence;
	if (index >= 1)
	{
		sequence = "(";
		sequence += std::to_string(index);
		sequence += ")";
	}

	m_emitterList.push_back(std::make_shared<EmitterGui>(emitter, "ParticleEmitter" + sequence));
}

void ParticleDemoScene::AddEmitter(EmitterInitData initData, std::string emitterName)
{
	auto emitter = m_emitterManager->AddEmitter(initData);
	int index = m_emitterManager->GetEmitterIndex(emitter);
	if (index == -1)
		assert(nullptr);

	std::string sequence;
	if (index >= 1)
	{
		sequence = "(";
		sequence += std::to_string(index);
		sequence += ")";
	}

	m_emitterList.push_back(std::make_shared<EmitterGui>(emitter, emitterName));
}

void ParticleDemoScene::Save()
{
	//名前設定
	m_isSaving = true;
	static char name[30];
	ImGui::Begin("Save", 0, ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::InputText("ParticleName", name, 30);
		if (ImGui::Button("Save"))
		{
			ParticleSerialize::ParticleSerialize(name,m_emitterList,m_savedParticles);
			m_isSaving = false;
		}ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			m_isSaving = false;
		}
	}
	ImGui::End();
}

void ParticleDemoScene::Load()
{
	ImGui::Begin("Load", 0, ImGuiWindowFlags_AlwaysAutoResize);
	{
		static int listbox_item_current = 1;
		ImGui::ListBox("##Particles", &listbox_item_current, StringGetter,
			m_savedParticles.data(), (int)m_savedParticles.size());
		if (ImGui::Button("Load"))
		{
			auto inputList = ParticleSerialize::ParticleDeserialize(m_savedParticles[listbox_item_current]);
			for (auto input : inputList)
			{
				AddEmitter(input.data, input.emitterName);
			}
			m_isLoading = false;
		}ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			m_isLoading = false;
		}
	}
	ImGui::End();
}


void ParticleDemoScene::InitDeserialize()
{
	std::ifstream os(PARTICLE_LIST_PATH, std::ios::in);
	if (!os)
		return;

	cereal::JSONInputArchive archive(os);

	ParticleSerialize::serialize(archive, m_savedParticles);
}
