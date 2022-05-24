#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Scene.h"

Manager* Manager::m_singleton = nullptr;	//インスタンス

Scene* Manager::m_scene;

void Manager::Init()
{
	Renderer::GetInstance()->Init();

	m_scene = new Scene();
	m_scene->Init();
}


void Manager::Uninit()
{	
	m_scene->Uninit();
	delete m_scene;

	Renderer::GetInstance()->Uninit();
}

void Manager::Update()
{
	m_scene->Update();
}

void Manager::Draw()
{
	Renderer::GetInstance()->Begin();

	m_scene->Draw();

	Renderer::GetInstance()->End();
}

Manager::~Manager()
{
	delete m_singleton;
	m_singleton = nullptr;
}

//シングルトン用インスタンス取得関数
Manager* Manager::GetInstance()
{
	if (m_singleton)
	{
		m_singleton = new Manager();
	}
	return m_singleton;
}
