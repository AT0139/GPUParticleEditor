#include "main.h"
#include "Renderer.h"
#include "Game.h"
#include "Polygon2D.h"
#include "Camera.h"
#include "MeshField.h"
#include "Player.h"
#include "Stair.h"
#include "Bullet.h"
#include "Enemy.h"
#include "SkyDome.h"
#include "Tree.h"
#include "input.h"
#include "Manager.h"
#include "Result.h"
#include "ResourceManager.h"
#include "audio.h"
#include "GameUI.h"
#include "DefenceBase.h"


#include "Bullet.h"

namespace MainGame
{
	void Game::Init()
	{
		//カメラ
		AddGameObject<Camera>(CAMERA);

		//3Dオブジェクト
		AddGameObject<SkyDome>(OBJECT);
		auto field = AddGameObject<MeshField>(OBJECT);

		auto basePos = Vector3(5.0f, 5.0f, 5.0f);
		basePos.y =field->GetHeight(basePos);
		AddGameObject<DefenceBase>(OBJECT)->GetComponent<Transform>()->SetPosition(basePos);

		AddGameObject<Player>(OBJECT);
		//AddGameObject<Enemy>(OBJECT)->GetComponent<Transform>()->SetPosition(Vector3(00.0f, 4.0f, -0.0f));
		//AddGameObject<Stair>(OBJECT)->GetComponent<Transform>()->SetPosition(Vector3(10.0f, 5.0f, 10.0f));

		AddGameObject<GameUI>(UI);
		//Audio* bgm = AddGameObject<Audio>(UI);
		//bgm->Load("asset\\audio\\bgm.wav");
		//bgm->Play(true);

		m_collisionManager = std::make_unique<CollisionManager>();
		m_serializeManger = std::make_unique<SerializeManager>();
		StaticDataTable::GetInstance();
	}

	void Game::Uninit()
	{
		Scene::Uninit();
		ResourceManager::GetInstance().Release();
	}

	void Game::Update()
	{
		Scene::Update();

		m_collisionManager->Update();
		if (GET_INPUT.GetKeyTrigger(KEY_CONFIG::RETURN))
		{
			Manager::GetInstance().SetScene<Result>();
		}

		ImGui::Begin("Serialize", 0, ImGuiWindowFlags_AlwaysAutoResize);
		{
			if (ImGui::Button("Enemy"))
			{
				AddGameObject<Enemy>(OBJECT)->GetComponent<Transform>()->SetPosition(Vector3(50.0f, 4.0f, -0.0f));
			}

			if (ImGui::Button("serialize"))
			{
				m_serializeManger->ToSerialize();
			}
			if (ImGui::Button("deserialize"))
			{
				m_serializeManger->ToDeserialize();
			}
		}
		ImGui::End();

	}
}