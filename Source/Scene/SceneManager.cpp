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
	Renderer::GetInstance().Begin();

	m_pScene->Draw();

	Renderer::GetInstance().End();
}

SceneManager::~SceneManager()
{}