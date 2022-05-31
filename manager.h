#pragma once

class Scene;

class Manager
{

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	
	static Manager* GetInstance();
	static Scene* GetScene() { return m_scene; };

private:
	Manager() {}
	~Manager();

	static class Scene* m_scene;

	static Manager* m_singleton;
};