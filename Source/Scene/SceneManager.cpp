#include "main.h"
#include "renderer.h"
#include "Scene.h"
#include "SceneManager.h"
#include "input.h"
#include "Game.h"
#include "Title.h"
#include "Result.h"
#include "CAudio.h"
#include "ParticleDemoScene.h"

SceneManager::SceneManager()
	: m_pScene(nullptr)
{}

void SceneManager::Init()
{
	Renderer::GetInstance().Init();
	GET_INPUT.Init();
	CAudio::InitMaster();

	SetScene<ParticleDemoScene>();
}

void SceneManager::Uninit()
{
	m_pScene->Uninit();
	delete m_pScene;

	CAudio::UninitMaster();
	GET_INPUT.Uninit();
	Renderer::GetInstance().Uninit();
}

void SceneManager::Update()
{
	GET_INPUT.Update();

	m_pScene->Update();
}

void SceneManager::Draw()
{
	auto camera = m_pScene->GetCamera();
	LIGHT light;
	light.Enable = true;
	light.Direction = Vector4(1.0f, -1.0f, 1.0f, 0.0f);
	light.Direction = XMVector4Normalize(light.Direction);
	light.Ambient = Color(0.2f, 0.2f, 0.2f, 1.0f);
	light.diffuse = Color(2.0f, 2.0f, 2.0f, 1.0f);
	light.view = camera->GetViewMatrix();
	//ライトカメラのプロジェクション行列を作成
	light.proj = camera->GetProjectionMatrix();
		//CreatePerspectiveFieldOfView(1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 5.0f, 100.0f);
	Renderer::GetInstance().SetLight(light);

	Renderer::GetInstance().BeginDepth();
	Renderer::GetInstance().SetViewMatrix(&light.view);
	Renderer::GetInstance().SetProjectionMatrix(&light.proj);
	m_pScene->DepthDraw();

	Renderer::GetInstance().Begin();

	m_pScene->Draw();

	Renderer::GetInstance().End();
}

SceneManager::~SceneManager()
{}