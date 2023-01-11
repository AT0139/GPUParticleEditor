#include "renderer.h"
#include "ParticleDemoSceneCamera.h"

static const float CAMERA_FACTOR = 120.0f;
static const float CAMERA_DISTANCE = 20.0f;

static const float DELTA_MIN = 2.0f;
static const float DELTA_MAX = 4.0f;

static const float FIELD_Y_OFFSET = 1.0f;


ParticleDemoSceneCamera::ParticleDemoSceneCamera()
{
	m_cameraPos = Vector3(0.0f, 0.0f, -10.0f);
}

ParticleDemoSceneCamera::~ParticleDemoSceneCamera()
{
}

void ParticleDemoSceneCamera::Update()
{
}