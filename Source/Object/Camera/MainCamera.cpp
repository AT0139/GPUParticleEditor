#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "MainCamera.h"
#include "input.h"
#include "Scene.h"
#include "Player.h"
#include "MeshField.h"

static const float CAMERA_FACTOR = 120.0f;
static const float CAMERA_DISTANCE = 5.0f;

static const float DELTA_MIN = 2.0f;
static const float DELTA_MAX = 4.0f;

static const float FIELD_Y_OFFSET = 1.0f;

namespace MainGame
{
	MainCamera::MainCamera()
		: m_targetYOffset(0.0f)
		, m_targetYOffsetTemporary(0.0f)
		, m_target(0.0f, 0.0f, 0.0f)
		, m_theta(4.57f)
		, m_delta(0.43f)
		, m_lerpRatio(0.0f)
		, m_isOnField(false)
	{
		m_cameraPos.x = m_target.x + CAMERA_DISTANCE * cos(m_delta) * cos(m_theta);
		m_cameraPos.y = m_target.y + CAMERA_DISTANCE * sin(m_delta);
		m_cameraPos.z = m_target.z + CAMERA_DISTANCE * cos(m_delta) * sin(m_theta);
	}

	MainCamera::~MainCamera()
	{
	}

	void MainCamera::Update()
	{
		//プレイヤーの取得
		Scene* scene = Manager::GetInstance().GetScene();
		Vector3 playerPos = scene->GetGameObject<Player>(scene->OBJECT)->GetComponent<Transform>()->GetPosition();

		m_target = playerPos;
		m_target.y += m_targetYOffset;

		//マウス加速度
		auto mouseAcc = GET_INPUT.GetMouseAcceleration();
		float mouseXAcc = mouseAcc.x / CAMERA_FACTOR;
		float mouseYAcc = mouseAcc.y / CAMERA_FACTOR;


		m_theta += mouseXAcc;
		m_delta += mouseYAcc;

		if (m_delta <= DELTA_MIN)
			m_delta = DELTA_MIN;

		if (m_delta >= DELTA_MAX)
			m_delta = DELTA_MAX;
	
		//カメラがフィールドに沿っていたら
		if (m_isOnField)
		{
			//上を向けるようにターゲット位置に加算する変数に加算
			m_targetYOffset += mouseYAcc;
			m_lerpRatio = 0.0f;
		}

		m_cameraPos.x = playerPos.x + CAMERA_DISTANCE * cos(m_delta) * cos(m_theta);
		m_cameraPos.y = playerPos.y + CAMERA_DISTANCE * sin(m_delta);
		m_cameraPos.z = playerPos.z + CAMERA_DISTANCE * cos(m_delta) * sin(m_theta);

		//フィールドにめりこまないように
		MainGame::MeshField* field = scene->GetGameObject<MainGame::MeshField>(scene->OBJECT);
		float fieldHeight = field->GetHeight(m_cameraPos);
		if (m_cameraPos.y < fieldHeight + FIELD_Y_OFFSET)
		{
			m_cameraPos.y = fieldHeight + FIELD_Y_OFFSET;
			m_isOnField = true;
			m_targetYOffsetTemporary = m_targetYOffset;
		}
		else
		{
			m_isOnField = false;

			//いきなりカメラが動かないように
			if (std::abs(m_targetYOffsetTemporary) > 0)
			{
				m_targetYOffset = Utility::Lerp(m_targetYOffsetTemporary, 0.0f, m_lerpRatio);
				m_lerpRatio += 0.05f;
				if (m_lerpRatio >= 1.0f)
				{
					m_lerpRatio = 0.0f;
					m_targetYOffsetTemporary = 0.0f;
					m_targetYOffset = 0.0f;
				}
			}
			else
				m_lerpRatio = 0.0f;
		}


		Renderer::GetInstance().SetCameraPosition(m_cameraPos);
	}


	void MainCamera::Draw()
	{
		//ビューマトリクス設定
		m_viewMatrix = XMMatrixLookAtLH(m_cameraPos, m_target, Vector3::Up);

		Renderer::GetInstance().SetViewMatrix(&m_viewMatrix);

		//プロジェクションマトリクス設定
		m_projection = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

		Renderer::GetInstance().SetProjectionMatrix(&m_projection);
	}
	Vector3 MainCamera::GetCamaraForward()
	{
		Vector3 forward = m_target - m_cameraPos;
		forward.y = 0.0f;
		return XMVector3Normalize(forward);
	}
	Vector3 MainCamera::GetCamaraRight()
	{
		Vector3 forward = m_target - m_cameraPos;

		Vector3 right;
		right.x = forward.z;
		right.y = 0.0f;
		right.z = -forward.x;
		
		return XMVector3Normalize(right);
	}


}