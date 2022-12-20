#pragma once

class GameObject;

class Component
{
public:
	explicit Component(GameObject* pGameObject);
	virtual ~Component() {}

	virtual void Update() = 0;
	virtual void Draw() = 0;

	GameObject* GetGameObject()const { return m_pGameObject; }
	void SetGameObject(GameObject* pGameObject) { m_pGameObject = pGameObject; }
private:
	GameObject* m_pGameObject;
};