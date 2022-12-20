#include "renderer.h"
#include "AnimationModel.h"
#include "audio.h"
#include "Player.h"
#include "input.h"
#include "ResourceManager.h"
#include "Rigidbody.h"
#include "Manager.h"
#include "BlankObject.h"
#include "Camera.h"

namespace MainGame
{
	float Player::m_blendRate = 0.0f;
	Player::Player()
		: m_haveObject(nullptr)
		, MOVE_SPEED(0.1f)
		, ADD_BLENDRATE(0.05f)
		, JUMP_FORCE(0.8f)
		, THROW_FORCE(0.1f)
	{
		m_tag = TAG::PLAYER;

		m_rigid = AddComponent<Rigidbody>();
		m_rigid->SetMass(1.5f);
		auto col = AddComponent<OBBCollision>();
		col->SetScale(0.5f);
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

		m_transform = GetComponent<Transform>();
		m_transform->SetScale(Vector3(0.008f, 0.008f, 0.008f));


		//当たり判定用子オブジェクト
		auto scene = Manager::GetInstance().GetScene();
		auto obj = scene->AddGameObject<BlankObject>(scene->OBJECT);
		auto objRigid = obj->AddComponent<Rigidbody>();
		objRigid->SetIsTrigger(true);
		objRigid->SetIsKinematic(true);
		auto objCol = obj->AddComponent<SphereCollision>();
		objCol->SetRadius(0.1f);
		objCol->SetHitAction(HitAction::None);
		auto objTrans = obj->GetComponent<Transform>();
		objTrans->SetParent(this);
		objTrans->SetPosition(-m_transform->GetForward() * 1.0f);
	}

	Player::~Player()	
	{
		m_vertexLayout->Release();
		m_vertexShader->Release();
		m_pixelShader->Release();
	}

	void Player::Update()
	{
		m_model->Update(m_animationName.c_str(), m_blendRate, m_frame);
		m_frame++;

		Move();

		if (GET_INPUT.GetKeyTrigger(KEY_CONFIG::JUMP) && m_rigid->IsGround())
		{
			Jump();
		}

		//投げる
		if (m_haveObject != nullptr)
		{
			if (GET_INPUT.GetKeyRelease(KEY_CONFIG::ACTION))
			{
				Throw();
			}
		}
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
			if (collision->GetTag() == TAG::ENEMY)
			{
				auto colTrans = collision->GetComponent<Transform>();
				m_haveObject = collision;
				colTrans->SetParent(this);
				colTrans->SetPosition(Vector3(0.0f, 1.0f, -1.0f));
				collision->GetComponent<Rigidbody>()->SetIsKinematic(true);
				collision->GetComponent<Rigidbody>()->SetIsTrigger(true);
			}
		}
	}

	void Player::OnTrigger(GameObject* collision)
	{

	}

	void Player::Move()
	{
		auto scene = Manager::GetInstance().GetScene();
		auto camera = scene->GetGameObject<Camera>(scene->CAMERA);

		Vector3 cameraForward = camera->GetCamaraForward();	//カメラ向き
		Vector3 cameraRight = camera->GetCamaraRight();	//カメラ向き

		cameraForward.y = 0.0f;
		cameraRight.y = 0.0f;

		//MOVE
		Vector3 velo = Vector3(0, 0, 0);
		if (GET_INPUT.GetKeyPress(KEY_CONFIG::MOVE_UP))
		{
			velo += cameraForward * MOVE_SPEED;
			m_animationName = "Run";
			
			m_transform->SetQuaternion(Quaternion::LookRotation(cameraForward, Vector3::Up));
			m_blendRate += ADD_BLENDRATE;
		}
		else if (GET_INPUT.GetKeyPress(KEY_CONFIG::MOVE_DOWN))
		{
			velo -= cameraForward * (MOVE_SPEED / 2);
			m_animationName = "WalkingBack";
			m_blendRate += ADD_BLENDRATE;
			m_transform->SetQuaternion(-Quaternion::LookRotation(cameraForward, Vector3::Up));
		}
		else
		{
			m_blendRate += ADD_BLENDRATE * 2;

			m_animationName = "Idle";
		}
		if (GET_INPUT.GetKeyPress(KEY_CONFIG::MOVE_LEFT))
		{
			velo -= cameraRight * MOVE_SPEED;
			m_transform->SetQuaternion(Quaternion::LookRotation(cameraForward, Vector3::Up));
		}
		if (GET_INPUT.GetKeyPress(KEY_CONFIG::MOVE_RIGHT))
		{
			velo += cameraRight * MOVE_SPEED;
			m_transform->SetQuaternion(Quaternion::LookRotation(cameraForward, Vector3::Up));
		}

		if (m_blendRate > 1.0f)
			m_blendRate = 1.0f;
		if (m_blendRate < 0.0f)
			m_blendRate = 0.0f;

		m_rigid->SetVelocity(velo);
	}


	void Player::Jump()
	{
		m_rigid->AddForce(Vector3::Up * JUMP_FORCE);
	}


	void Player::Throw()
	{
		auto haveTrans = m_haveObject->GetComponent<Transform>();
		haveTrans->ResetParent();

		auto haveRigid = m_haveObject->GetComponent<Rigidbody>();
		haveRigid->SetIsKinematic(false);
		haveRigid->SetIsTrigger(false);
		auto forward = this->GetComponent<Transform>()->GetForward();
		forward.Normalize();
		//力を前方向に
		haveRigid->AddForce(-forward * THROW_FORCE);
		m_haveObject = nullptr;
	}
}