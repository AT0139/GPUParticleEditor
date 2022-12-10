#include "renderer.h"
#include "AnimationModel.h"
#include "Bullet.h"
#include "audio.h"
#include "Player.h"
#include "input.h"
#include "ResourceManager.h"
#include "Rigidbody.h"
#include "Manager.h"
#include "BlankObject.h"
#include "Stair.h"

static const float CAMERA_FACTOR = 60.0f;

namespace MainGame
{
	float Player::m_blendRate = 0.0f;
	Player::Player()
		: m_haveObject(nullptr)
	{
		m_rigid = AddComponent<Rigidbody>();
		m_rigid->SetMass(2.0f);
		AddComponent<SphereCollision>()->SetRadius(0.5f);

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
		//m_shadow->SetScale(Vector3(2.0f, 1.0f, 2.0f));

		auto tranform = GetComponent<Transform>();
		tranform->SetScale(Vector3(0.008f, 0.008f, 0.008f));

		m_tag = TAG::PLAYER;

		//当たり判定用子オブジェクト
		auto scene = Manager::GetInstance().GetScene();
		auto obj = scene->AddGameObject<BlankObject>(scene->OBJECT);
		auto objRigid = obj->AddComponent<Rigidbody>();
		objRigid->SetIsTrigger(true);
		objRigid->SetIsKinematic(true);
		obj->AddComponent<SphereCollision>()->SetRadius(0.1f);
		auto objTrans = obj->GetComponent<Transform>();
		objTrans->SetParent(this);
		objTrans->SetPosition(Vector3(0.0f,0.0f,1.0f));
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

		Vector3 forward = transform->GetForward();
		Vector3 right = transform->GetRight();

		Vector3 pos = transform->GetPosition();
		Quaternion rot;

		//マウス
		{
			m_preMousePos = m_mousePos;
			m_mousePos = GetMousePos();

			float mouseXAcc = (m_preMousePos.x - m_mousePos.x) / CAMERA_FACTOR;
			float mouseYAcc = (m_preMousePos.y - m_mousePos.y) / CAMERA_FACTOR;

			auto yAxiz = Vector3(0.0f, 1.0f, 0.0f);
			rot = XMQuaternionRotationAxis(yAxiz, XMConvertToRadians(-mouseXAcc * 20.0f));

			//todo : マウスでカメラ回転上下
			//m_rotation.x -= mouseYAcc;
		}
		//MOVE
		{
			Vector3 velo = Vector3(0, 0, 0);
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

			m_rigid->SetVelocity(velo);
		}
		transform->AddQuaternion(rot);

#ifdef _DEBUG
		ImGui::Begin("General");
		{
			ImGui::Text("x = %d  y = %d", m_mousePos.x, m_mousePos.y);
			ImGui::Text("x = %f  y = %f", rot.x, rot.y);
		}
		ImGui::End();
#endif
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
		Matrix world = GetComponent<Transform>()->GetWorldMatrix();
		Renderer::GetInstance().SetWorldMatrix(&world);

		m_model->Draw();
	}

	void Player::OnCollision(GameObject* collision)
	{
		if (m_haveObject == nullptr)
		{
			auto colTrans = collision->GetComponent<Transform>();
			m_haveObject = collision;
			colTrans->SetParent(this);
			colTrans->SetPosition(Vector3(0.0f, 1.0f, 1.0f));
			collision->GetComponent<Rigidbody>()->SetIsKinematic(true);
		}
	}

	void Player::OnTrigger(GameObject* collision)
	{

	}
}