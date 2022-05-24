#pragma once


class Manager
{

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	
	static Manager* GetInstance();


private:
	Manager() {}
	~Manager();

	static class Scene* m_scene;

	static Manager* m_singleton;
};