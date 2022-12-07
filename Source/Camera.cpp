#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Camera.h"
#include "input.h"
#include "Scene.h"
#include "Player.h"

namespace MainGame
{
	Camera::Camera()
	{
		auto transform = GetComponent<Transform>();
		transform->SetPosition(Vector3(0.0f, 5.0f, -5.0f));
		m_target = Vector3(0.0f, 0.0f, 0.0f);
	}

	Camera::~Camera()
	{
	}

	void Camera::Update()
	{
		auto transform = GetComponent<Transform>();
		//プレイヤーの取得
		Scene* scene = Manager::GetInstance().GetScene();
		auto playerTransform = scene->GetGameObject<Player>(scene->OBJECT)->GetComponent<Transform>();
		Vector3 playerPos = playerTransform->GetPosition();
		Vector3 playerForward = playerTransform->GetForward();
		Vector3 playerRight = playerTransform->GetRight();

		//トップビュー
		//m_target = playerPos;
		//m_position = playerPos + Vector3(0.0f, 5.0f, -5.0f);

		//サードパーソンビュー
		m_target = Vector3(playerPos.x, playerPos.y + m_targetYoffset, playerPos.z);
		transform->SetPosition(playerPos - playerForward * 5.0f + Vector3(0.0f, 2.5f + m_positionYoffset, 0.0f));

		//サードパーソンビュー(右寄り)
		//m_target = playerPos + playerRight;
		//m_position = playerPos - playerForward * 5.0f + playerRight + Vector3(0.0f, 2.0f, 0.0f);

		////ファーストパーソンビュー
		//m_target = playerPos + playerForward;
		//m_position = playerPos;

		Renderer::GetInstance().SetCameraPosition(transform->GetPosition());
	}

	void Camera::Draw()
	{
		auto position = GetComponent<Transform>()->GetPosition();

		//ビューマトリクス設定
		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
		m_viewMatrix = XMMatrixLookAtLH(position, m_target, up);

		Renderer::GetInstance().SetViewMatrix(&m_viewMatrix);

		//プロジェクションマトリクス設定
		Matrix projectionMatrix = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

		Renderer::GetInstance().SetProjectionMatrix(&projectionMatrix);
	}
}