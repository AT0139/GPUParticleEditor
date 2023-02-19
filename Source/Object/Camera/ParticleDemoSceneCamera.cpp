#include "Renderer.h"
#include "ParticleDemoSceneCamera.h"


ParticleDemoSceneCamera::ParticleDemoSceneCamera()
{
	m_cameraPos = Vector3(0.0f, 0, -50.0f);
	m_target = Vector3(0.0f, 0, 0.0f);
	Renderer::GetInstance().SetCameraPosition(m_cameraPos);

}

ParticleDemoSceneCamera::~ParticleDemoSceneCamera()
{
}

void ParticleDemoSceneCamera::Update()
{
}