#include "ParticleDemoScene.h"
#include "Title.h"
#include "Input.h"
#include "SceneManager.h"
#include "SkyDome.h"
#include "ParticleDemoSceneCamera.h"
#include "Field.h"
#include "Polygon2D.h"

static const std::string PARTICLE_FOLDER_PATH = "./Particles/";
static const std::string PARTICLE_LIST_PATH = PARTICLE_FOLDER_PATH + "Particles.json";

void ParticleDemoScene::Init()
{
	GET_INPUT.ToggleCursor(true);

	//カメラ
	SetCamera<ParticleDemoSceneCamera>();
	AddGameObject<SkyDome>(OBJECT)->GetComponent<Transform>()->SetPosition(Vector3(0.0f, -00.0f, 0.0f));

	m_emitterManager = AddGameObject<ParticleEmitterManager>(EFFECT);

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

	//メインウィンドウ
	ImGui::Begin("ParticleSetting", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking);
	{
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
					ToDeserialize();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (!m_isSaving)
		{
			//エミッタの追加
			if (ImGui::Button("AddEmitter"))
			{
				AddEmitter();
			}

			static float y = 0.0f;
			static float z = 0.0f;
			ImGui::SliderFloat("y", &y, -50.0f, 50.0f);
			ImGui::SliderFloat("z", &z, -200.0f, 200.0f);
			m_emitterManager->GetComponent<Transform>()->SetPosition(Vector3(0.0f, y, z));
		}
	}
	ImGui::End();

	//セーブ処理中
	if (m_isSaving)
	{
		//名前設定
		m_isSaving = true;
		static char name[30];
		ImGui::Begin("Save", 0, ImGuiWindowFlags_AlwaysAutoResize);
		{
			ImGui::InputText("ParticleName", name, 30);
			if (ImGui::Button("Save"))
			{
				ToSerialize(name);
				m_isSaving = false;
			}
		}
		ImGui::End();
	}
	else
	{
		for (auto it : m_emitterList)
		{
			ImGui::PushID(&it);
			it->Update();
			ImGui::PopID();
		}

		if (GET_INPUT.GetKeyTrigger(KEY_CONFIG::RETURN))
		{
			SceneManager::GetInstance().SetScene<Title>();
		}
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

//シリアライズ
void ParticleDemoScene::ToSerialize(std::string particleName)
{
	std::list<ParticleSerializeData> serializeList;

	for (auto emitter : m_emitterList)
	{
		ParticleSerializeData serializeData;
		serializeData.data = emitter->GetEmitter()->GetSerializeData();
		serializeData.emitterName = emitter->GetEmitterName();
		serializeList.push_back(serializeData);
	}

	{
		std::ofstream os(PARTICLE_LIST_PATH, std::ios::out);
		cereal::JSONOutputArchive archiveFile(os);

		m_savedParticles.push_back(particleName);
		serialize(archiveFile, m_savedParticles);
	}

	{
		std::ofstream os(PARTICLE_FOLDER_PATH + particleName + ".json", std::ios::out);
		cereal::JSONOutputArchive archiveFile(os);
		serialize(archiveFile, serializeList);
	}
}

//デシリアライズ
void ParticleDemoScene::ToDeserialize()
{
	std::ifstream os(PARTICLE_LIST_PATH, std::ios::in);
	cereal::JSONInputArchive archive(os);

	std::list<ParticleSerializeData> inputList;

	serialize(archive, inputList);
	for (auto input : inputList)
	{
		AddEmitter(input.data, input.emitterName);
	}
}