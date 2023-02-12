#include "ParticleDemoScene.h"
#include "Title.h"
#include "Input.h"
#include "SceneManager.h"
#include "SkyDome.h"
#include "ParticleDemoSceneCamera.h"
#include "Field.h"
#include "Polygon2D.h"

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
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
				{
					ToSerialize();
				}
				if (ImGui::MenuItem("Load"))
				{

				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (ImGui::Button("AddEmitter"))
		{
			AddEmitter();
		}

		//todo : リセット

		static float y = 0.0f;
		static float z = 0.0f;
		ImGui::SliderFloat("y", &y, -50.0f, 50.0f);
		ImGui::SliderFloat("z", &z, -200.0f, 200.0f);
		m_emitterManager->GetComponent<Transform>()->SetPosition(Vector3(0.0f, y, z));
	}
	ImGui::End();


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

void ParticleDemoScene::ToSerialize()
{
	std::list<ParticleSerializeData> serializeList;

	for (auto emitter : m_emitterList)
	{
		ParticleSerializeData serializeData;
		serializeData.data = emitter->GetEmitter()->GetSerializeData();
		serializeList.push_back(serializeData);

	}
	std::ofstream os("Particle.json", std::ios::out);
	cereal::JSONOutputArchive archiveFile(os);

	serialize(archiveFile, serializeList);
}
