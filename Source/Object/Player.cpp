#include "renderer.h"
#include "DrawAnimationModel.h"
#include "audio.h"
#include "Player.h"
#include "input.h"
#include "ResourceManager.h"
#include "Rigidbody.h"
#include "SceneManager.h"
#include "ChildObject.h"
#include "MainCamera.h"

namespace MainGame
{
	Player::Player()
		: m_haveObject(nullptr)
		, MOVE_SPEED(0.1f)
		, JUMP_FORCE(0.8f)
		, THROW_FORCE(0.1f)
	{
		SetTag(TAG::PLAYER);

		m_rigid = AddComponent<Rigidbody>();
		m_rigid->SetMass(1.5f);
		auto col = AddComponent<OBBCollision>();
		col->SetScale(Vector3(0.5f, 1.6f, 0.5f));
		col->SetCenterPosition(Vector3(0.0f, 0.8f, 0.0f));
		//モデル読み込み
		m_model = AddComponent<DrawAnimationModel>(this);
		m_model->Load("asset\\model\\Akai_Idle.fbx");
		m_model->LoadAnimation("asset\\model\\Akai_Idle.fbx", "Idle");
		m_model->LoadAnimation("asset\\model\\Akai_Run.fbx", "Run");
		m_model->LoadAnimation("asset\\model\\Akai_WalkingBackward.fbx", "WalkingBack");

		//m_shotSE = scene->AddGameObject<Audio>(scene->UI);
		//m_shotSE->Load("asset\\audio\\wan.wav");

		//m_shadow = scene->AddGameObject<Shadow>(scene->OBJECT);
		//m_shadow->SetPosition(m_position);
		//m_shadow->SetScale(Vector3(2.0f, 1.0f, 2.0f));

		m_transform = GetComponent<Transform>();
		m_transform->SetScale(Vector3(0.008f, 0.008f, 0.008f));


		//当たり判定用子オブジェクト
		auto scene = SceneManager::GetInstance().GetScene();
		auto obj = scene->AddGameObject<ChildObject>(scene->OBJECT);
		auto objRigid = obj->AddComponent<Rigidbody>();
		objRigid->SetIsTrigger(true);
		objRigid->SetIsKinematic(true);
		auto objCol = obj->AddComponent<SphereCollision>();
		objCol->SetScale(0.1f);
		objCol->SetHitAction(HitAction::None);
		auto objTrans = obj->GetComponent<Transform>();
		objTrans->SetParent(this);
		objTrans->SetPosition(-m_transform->GetForward() * 1.0f);
	}

	Player::~Player()	
	{
	}

	void Player::Update()
	{
		Move();

		if (GET_INPUT.GetKeyTrigger(KEY_CONFIG::JUMP) && m_rigid->IsGround())
		{
			Jump();
		}

		//投げる
		if (m_haveObject != nullptr)
		{
			if (GET_INPUT.GetKeyRelease(KEY_CONFIG::DETERMINATION))
			{
				Throw();
			}
		}
	}

	void Player::Draw()
	{
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

		if (collision->GetTag() == TAG::STATIC_OBJECT)
		{
			auto transform = collision->GetComponent<Transform>();
			float colYpos = transform->GetPosition().y;
			float myYpos = m_transform->GetPosition().y;

			if (myYpos > colYpos)
				m_rigid->SetIsGround(true);
		}
	}

	void Player::OnTrigger(GameObject* collision)
	{

	}

	void Player::Move()
	{
		auto scene = SceneManager::GetInstance().GetScene();
		auto camera = scene->GetGameObject<MainCamera>(scene->CAMERA);

		Vector3 cameraForward = camera->GetCamaraForward();	//カメラ向き
		Vector3 cameraRight = camera->GetCamaraRight();	//カメラ向き

		cameraForward.y = 0.0f;
		cameraRight.y = 0.0f;

		//MOVE
		Vector3 velo = Vector3(0, 0, 0);
		if (GET_INPUT.GetKeyPress(KEY_CONFIG::MOVE_UP))
		{
			velo += cameraForward * MOVE_SPEED;
			m_model->SetAnimationName("Run");
			
			m_transform->SetQuaternion(Quaternion::LookRotation(cameraForward, Vector3::Up));
			m_model->AddBlendRate();
		}
		else if (GET_INPUT.GetKeyPress(KEY_CONFIG::MOVE_DOWN))
		{
			velo -= cameraForward * (MOVE_SPEED / 2);
			m_model->SetAnimationName("WalkingBack");
			m_model->AddBlendRate();
			m_transform->SetQuaternion(-Quaternion::LookRotation(cameraForward, Vector3::Up));
		}
		else
		{
			m_model->AddBlendRate(2);

			m_model->SetAnimationName("Idle");
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