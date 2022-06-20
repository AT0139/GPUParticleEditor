#pragma once

#include "Scene.h"

class Manager
{

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	
	static Manager* GetInstance();
	static Scene* GetScene() { return m_scene; };

	template<typename T>
	static void SetScene()
	{
		if (m_scene)
		{
			m_scene->Uninit();
			delete m_scene;
		}
		m_scene = new T();
		m_scene->Init();
	}

private:
	Manager() {}
	~Manager();

	static class Scene* m_scene;

	static Manager* m_singleton;
};