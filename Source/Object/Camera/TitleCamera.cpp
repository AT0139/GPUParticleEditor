#include "renderer.h"
#include "TitleCamera.h"

static const float CAMERA_FACTOR = 120.0f;
static const float CAMERA_DISTANCE = 20.0f;

static const float DELTA_MIN = 2.0f;
static const float DELTA_MAX = 4.0f;

static const float FIELD_Y_OFFSET = 1.0f;


TitleCamera::TitleCamera()
	: m_targetYOffset(0.0f)
	, m_targetYOffsetTemporary(0.0f)
	, m_theta(3.57f)
	, m_delta(3.43f)
	, m_lerpRatio(0.0f)
	, m_isOnField(false)
{
	m_cameraPos.x = m_target.x + CAMERA_DISTANCE * cos(m_delta) * cos(m_theta);
	m_cameraPos.y = m_target.y + CAMERA_DISTANCE * sin(m_delta);
	m_cameraPos.z = m_target.z + CAMERA_DISTANCE * cos(m_delta) * sin(m_theta);
}

TitleCamera::~TitleCamera()
{
}

void TitleCamera::Update()
{
	m_target = Vector3(0.0f, 10.0f, 0.0f);
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