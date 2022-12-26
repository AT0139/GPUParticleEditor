#include "main.h"
#include "renderer.h"
#include "Scene.h"
#include "manager.h"
#include "input.h"
#include "Game.h"
#include "Title.h"
#include "Result.h"
#include "CAudio.h"

Manager::Manager()
	: m_pScene(nullptr)
{}

void Manager::Init()
{
	Renderer::GetInstance().Init();
	GET_INPUT.Init();
	CAudio::InitMaster();

	SetScene<MainGame::Game>();
}

void Manager::Uninit()
{
	m_pScene->Uninit();
	delete m_pScene;

	CAudio::UninitMaster();
	GET_INPUT.Uninit();
	Renderer::GetInstance().Uninit();
}

void Manager::Update()
{
	GET_INPUT.Update();

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