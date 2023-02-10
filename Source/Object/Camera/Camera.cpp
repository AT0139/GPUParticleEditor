#include "renderer.h"
#include "Camera.h"

static const float CAMERA_FACTOR = 120.0f;
static const float CAMERA_DISTANCE = 20.0f;

static const float DELTA_MIN = 2.0f;
static const float DELTA_MAX = 4.0f;

static const float FIELD_Y_OFFSET = 1.0f;


Camera::Camera()
	: m_viewMatrix(Matrix::Identity)
{}

Camera::~Camera()
{
}

void Camera::Update()
{
}


void Camera::Draw()
{
	//ビューマトリクス設定
	m_viewMatrix = XMMatrixLookAtLH(m_cameraPos, m_target, Vector3::Up);

	Renderer::GetInstance().SetViewMatrix(&m_viewMatrix);

	//プロジェクションマトリクス設定
	m_projection = XMMatrixPerspectiveFovLH(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 500.0f);

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