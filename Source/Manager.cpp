#include "main.h"
#include "renderer.h"
#include "Scene.h"
#include "manager.h"
#include "input.h"
#include "Game.h"
#include "Title.h"
#include "Result.h"
#include "audio.h"

Manager::Manager()
	: m_pScene(nullptr)
{}

void Manager::Init()
{
	Renderer::GetInstance().Init();
	Input::Init();
	Audio::InitMaster();

	SetScene<Title>();
}

void Manager::Uninit()
{
	m_pScene->Uninit();
	delete m_pScene;

	Audio::UninitMaster();
	Input::Uninit();
	Renderer::GetInstance().Uninit();
}

void Manager::Update()
{
	Input::Update();

	m_pScene->Update();
}

void Manager::Draw()
{
	Renderer::GetInstance().Begin();

	m_pScene->Draw();

	Renderer::GetInstance().End();
}

Manager::~Manager()
{}