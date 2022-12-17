#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Camera.h"
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
	Camera::Camera()
	{
		m_target = Vector3(0.0f, 0.0f, 0.0f);
		m_theta = 4.57f;
		m_delta = 0.43f;

		m_cameraPos.x = m_target.x + CAMERA_DISTANCE * cos(m_delta) * cos(m_theta);
		m_cameraPos.y = m_target.y + CAMERA_DISTANCE * sin(m_delta);
		m_cameraPos.z = m_target.z + CAMERA_DISTANCE * cos(m_delta) * sin(m_theta);
	}

	Camera::~Camera()
	{
	}

	void Camera::Update()
	{
		//プレイヤーの取得
		Scene* scene = Manager::GetInstance().GetScene();
		Vector3 playerPos = scene->GetGameObject<Player>(scene->OBJECT)->GetComponent<Transform>()->GetPosition();

		m_target = playerPos;

		//マウス位置取得
		m_preMousePos = m_mousePos;
		m_mousePos = GetMousePos();
		if (Input::GetKeyPress(KEY_CONFIG::MOUSE_R))
		{
			//マウス加速度
			float mouseXAcc = (m_preMousePos.x - m_mousePos.x) / CAMERA_FACTOR;
			float mouseYAcc = (m_preMousePos.y - m_mousePos.y) / CAMERA_FACTOR;

			m_theta += mouseXAcc;
			m_delta += mouseYAcc;

			if (m_delta <= DELTA_MIN)
				m_delta = DELTA_MIN;

			if (m_delta >= DELTA_MAX)
				m_delta = DELTA_MAX;
		}
		m_cameraPos.x = m_target.x + CAMERA_DISTANCE * cos(m_delta) * cos(m_theta);
		m_cameraPos.y = m_target.y + CAMERA_DISTANCE * sin(m_delta);
		m_cameraPos.z = m_target.z + CAMERA_DISTANCE * cos(m_delta) * sin(m_theta);

		//フィールドにめりこまないように
		MainGame::MeshField* field = scene->GetGameObject<MainGame::MeshField>(scene->OBJECT);
		float fieldHeight = field->GetHeight(m_cameraPos);
		if (m_cameraPos.y  < fieldHeight + FIELD_Y_OFFSET)
		{
			m_cameraPos.y = fieldHeight + FIELD_Y_OFFSET;
		}

		Renderer::GetInstance().SetCameraPosition(m_cameraPos);
	}


	void Camera::Draw()
	{
		//ビューマトリクス設定
		m_viewMatrix = XMMatrixLookAtLH(m_cameraPos, m_target, Vector3::Up);

		Renderer::GetInstance().SetViewMatrix(&m_viewMatrix);

		//プロジェクションマトリクス設定
		m_projection = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

		Renderer::GetInstance().SetProjectionMatrix(&m_projection);
	}
	Vector3 Camera::GetCamaraForward()
	{
		Vector3 forward = m_target - m_cameraPos;
		forward.y = 0.0f;
		return XMVector3Normalize(forward);
	}
	Vector3 Camera::GetCamaraRight()
	{
		Vector3 forward = m_target - m_cameraPos;

		Vector3 right;
		right.x = forward.z;
		right.y = 0.0f;
		right.z = -forward.x;
		
		return XMVector3Normalize(right);
	}


}