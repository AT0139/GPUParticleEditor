#include "renderer.h"
#include "AnimationModel.h"
#include "Scene.h"
#include "manager.h"
#include "Bullet.h"
#include "audio.h"
#include "Player.h"
#include "input.h"
#include "ResourceManager.h"
#include "MeshField.h"
#include "SphereCollision.h"
#include "OBBCollision.h"
#include "Rigidbody.h"

static const float CAMERA_FACTOR = 60.0f;

namespace MainGame
{
	float Player::m_blendRate = 0.0f;
	Player::Player()
	{
		AddComponent<OBBCollision>()->SetCollisionScale(D3DXVECTOR3(2.0f,2.0f,2.0f));
		m_rigid = AddComponent<Rigidbody>();

		//モデル読み込み
		m_model = ResourceManager::GetInstance().GetAnimationModelData("asset\\model\\Akai_Idle.fbx");
		m_model->LoadAnimation((char*)"asset\\model\\Akai_Idle.fbx", "Idle");
		m_model->LoadAnimation((char*)"asset\\model\\Akai_Run.fbx", "Run");
		m_model->LoadAnimation((char*)"asset\\model\\Akai_WalkingBackward.fbx", "WalkingBack");
		
		m_animationName = "Idle";

		Renderer::GetInstance().CreateVertexShader(&m_vertexShader, &m_vertexLayout, "vertexLightingVS.cso");

		Renderer::GetInstance().CreatePixelShader(&m_pixelShader, "vertexLightingPS.cso");

		//m_shotSE = scene->AddGameObject<Audio>(scene->UI);
		//m_shotSE->Load("asset\\audio\\wan.wav");

		//m_shadow = scene->AddGameObject<Shadow>(scene->OBJECT);
		//m_shadow->SetPosition(m_position);
		//m_shadow->SetScale(D3DXVECTOR3(2.0f, 1.0f, 2.0f));

		GetComponent<Transform>()->SetScale(D3DXVECTOR3(0.008f, 0.008f, 0.008f));

		m_tag = TAG::PLAYER;
	}

	Player::~Player()
	{
		//m_model->Unload();
		//delete m_model;

		m_vertexLayout->Release();
		m_vertexShader->Release();
		m_pixelShader->Release();
	}

	void Player::Update()
	{
		auto transform = GetComponent<Transform>();
		m_model->Update(m_animationName.c_str(), m_blendRate, m_frame);

		D3DXVECTOR3 forward = transform->GetForward();
		D3DXVECTOR3 right = transform->GetRight();

		D3DXVECTOR3 pos = transform->GetPosition();
		D3DXVECTOR3 rot = transform->GetRotation();

		//!マウス
		{
			m_preMousePos = m_mousePos;
			m_mousePos = GetMousePos();

#ifdef _DEBUG
			ImGui::Begin("General");
			{
				ImGui::Text("x = %d  y = %d", m_mousePos.x, m_mousePos.y);
			}
			ImGui::End();
#endif
			float mouseXAcc = (m_preMousePos.x - m_mousePos.x) / CAMERA_FACTOR;
			float mouseYAcc = (m_preMousePos.y - m_mousePos.y) / CAMERA_FACTOR;

			rot.y -= mouseXAcc;
			//todo : マウスでカメラ回転上下
			//m_rotation.x -= mouseYAcc;
		}
		//!MOVE
		{
			D3DXVECTOR3 velo = D3DXVECTOR3(0, 0, 0);
			if (Input::GetKeyPress(KEY_CONFIG::MOVE_UP))
			{
				velo += forward * MOVE_SPEED;
				//pos += forward * MOVE_SPEED;
				m_animationName = "Run";

				m_blendRate += ADD_BLENDRATE;
			}
			else if (Input::GetKeyPress(KEY_CONFIG::MOVE_DOWN))
			{
				velo -= forward * (MOVE_SPEED / 2);
				m_animationName = "WalkingBack";
				m_blendRate += ADD_BLENDRATE;
			}
			else
			{
				m_blendRate += ADD_BLENDRATE * 2;
				m_animationName = "Idle";
			}
			if (Input::GetKeyPress(KEY_CONFIG::MOVE_LEFT))
				velo -= right * MOVE_SPEED;
			if (Input::GetKeyPress(KEY_CONFIG::MOVE_RIGHT))
				velo += right * MOVE_SPEED;

			if (m_blendRate > 1.0f)
				m_blendRate = 1.0f;
			if (m_blendRate < 0.0f)
				m_blendRate = 0.0f;

			Scene* scene = Manager::GetInstance().GetScene();
			MeshField* field = scene->GetGameObject<MeshField>(scene->OBJECT);
			pos.y = field->GetHeight(pos);
			m_rigid->SetVelocity(velo);
		}
		transform->SetPosition(pos);
		transform->SetRotation(rot);

		//SHOT
		//if (Input::GetKeyTrigger(KEY_CONFIG::RETURN))
		//{
		//	Scene* scene = Manager::GetInstance().GetScene();
		//	scene->AddGameObject<Bullet>(scene->OBJECT)->GetComponent<Transform>()->SetPosition(m_position);

		//	//m_shotSE->Play();
		//}

		m_frame++;
	}

	void Player::Draw()
	{
		//入力レイアウト設定
		Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_vertexLayout);

		//シェーダー設定
		Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
		Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

		//ワールドマトリクス設定
		D3DXMATRIX world = GetComponent<Transform>()->GetWorldMatrix();
		Renderer::GetInstance().SetWorldMatrix(&world);

		m_model->Draw();
	}

	void Player::OnCollision(GameObject* collision)
	{
		ImGui::Begin("General");
		{
			ImGui::Text("Coll!");

		}
		ImGui::End();
	}
}