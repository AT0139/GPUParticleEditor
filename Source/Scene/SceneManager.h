#pragma once

#include "Singleton.h"
#include "Scene.h"

class SceneManager : public Singleton<SceneManager>
{
public:
	friend class Singleton<SceneManager>;

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
	SceneManager();
	~SceneManager();

	class Scene* m_pScene;
};