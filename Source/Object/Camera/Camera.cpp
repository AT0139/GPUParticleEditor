#include "main.h"
#include "SceneManager.h"
#include "renderer.h"
#include "Camera.h"
#include "input.h"
#include "Scene.h"
#include "Player.h"
#include "MeshField.h"

static const float CAMERA_FACTOR = 120.0f;
static const float CAMERA_DISTANCE = 20.0f;

static const float DELTA_MIN = 2.0f;
static const float DELTA_MAX = 4.0f;

static const float FIELD_Y_OFFSET = 1.0f;


Camera::Camera()
	: m_targetYOffset(0.0f)
	, m_targetYOffsetTemporary(0.0f)
	, m_target(0.0f, 0.0f, 0.0f)
	, m_theta(3.57f)
	, m_delta(3.43f)
	, m_lerpRatio(0.0f)
	, m_isOnField(false)
	, m_viewMatrix(Matrix::Identity)
{
	m_cameraPos.x = m_target.x + CAMERA_DISTANCE * cos(m_delta) * cos(m_theta);
	m_cameraPos.y = m_target.y + CAMERA_DISTANCE * sin(m_delta);
	m_cameraPos.z = m_target.z + CAMERA_DISTANCE * cos(m_delta) * sin(m_theta);
}

Camera::~Camera()
{
}

void Camera::Update()
{
	m_target = Vector3(0.0f,10.0f,0.0f);
	m_target.y += m_targetYOffset;

	m_theta += 0.001f;

	if (m_delta <= DELTA_MIN)
		m_delta = DELTA_MIN;

	if (m_delta >= DELTA_MAX)
		m_delta = DELTA_MAX;

	m_cameraPos.x = m_target.x + CAMERA_DISTANCE * cos(m_delta) * cos(m_theta);
	m_cameraPos.y = m_target.y + CAMERA_DISTANCE * sin(m_delta);
	m_cameraPos.z = m_target.z + CAMERA_DISTANCE * cos(m_delta) * sin(m_theta);

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