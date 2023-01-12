#include "Renderer.h"
#include "ParticleDemoSceneCamera.h"


ParticleDemoSceneCamera::ParticleDemoSceneCamera()
{
	m_cameraPos = Vector3(0.0f, 100.0f, -50.0f);
	m_target = Vector3(0.0f, 100.0f, 0.0f);
}

ParticleDemoSceneCamera::~ParticleDemoSceneCamera()
{
}

void ParticleDemoSceneCamera::Update()
{
}