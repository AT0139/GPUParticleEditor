#pragma once

#include "Singleton.h"
#include "Scene.h"

class Manager : public Singleton<Manager>
{

public:
	friend class Singleton<Manager>;

	void Init();
	void Uninit();
	void Update();
	void Draw();

	Scene* GetScene() { return m_pScene; };

	template<typename T>
	void SetScene()
	{
		if (m_pScene)
		{
			m_pScene->Uninit();
			delete m_pScene;
		}
		m_pScene = new T();
		m_pScene->Init();
	}

private:
	Manager();
	~Manager();

	class Scene* m_pScene;
};