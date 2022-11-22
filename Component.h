#pragma once

class GameObject;

class Component
{
public:
	explicit Component(const std::shared_ptr<GameObject> pGameObject);
	virtual ~Component() {}

	std::shared_ptr<GameObject> GetGameObject()const;
	void SetGameObject(const std::shared_ptr<GameObject>& pGameObject) { m_pGameObject = pGameObject; }
private:
	std::weak_ptr<GameObject> m_pGameObject;

};