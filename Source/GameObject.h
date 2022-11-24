#pragma once

#include "Component.h"
#include "Transform.h"

using std::shared_ptr;

class GameObject
{
public:
	GameObject() { AddComponent<Transform>(); }
	virtual ~GameObject() {}

	//純粋仮想関数
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void SetDestroy() { m_destory = true; }
	bool Destroy()
	{
		if (m_destory)
		{
			delete this;
			return true;
		}
		else 
			return false;
	}

	template<typename T>
	shared_ptr<T> GetComponent() const
	{

	}

	template<>
	shared_ptr<Transform> GetComponent<Transform>()const
	{
		assert(m_transform);
		return m_transform;
	}

	template<typename T,typename... U>
	shared_ptr<T> AddComponent(U&&... param)
	{
		std::shared_ptr<T> ptr = shared_ptr<T>(new T(param...));
		std::type_index typeIndex = typeid(T);
		m_componentMap[typeIndex] = ptr;
		ptr->SetGameObject(this);
		return ptr;
	}

	template<>
	shared_ptr<Transform> AddComponent<Transform>()
	{
		std::shared_ptr<Transform> ptr(new Transform(this));
		ptr->SetGameObject(this);
		m_transform = ptr;
		return m_transform;
	}

	void ComponentUpdate()
	{
		for (auto comp : m_componentMap)
		{
			comp.second->Update();
		}
	}
	void ComponentDraw()
	{
		for (auto comp : m_componentMap)
		{
			comp.second->Draw();
		}
	}

private:
	std::map<std::type_index, std::shared_ptr<Component>> m_componentMap;
	shared_ptr<Transform> m_transform;
	bool m_destory = false;
};