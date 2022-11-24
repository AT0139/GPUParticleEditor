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
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	void SetDestroy() { m_destory = true; }
	bool Destroy()
	{
		if (m_destory)
		{
			Uninit();
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

	template<typename T>
	shared_ptr<T> AddComponent()
	{
		std::shared_ptr<T> ptr(new T);
		std::type_index typeIndex = typeid(T);
		m_componentMap[typeIndex] = ptr;
		ptr.SetGameObject(this);
	}

	template<>
	shared_ptr<Transform> AddComponent()
	{
		std::shared_ptr<Transform> ptr(new Transform(this));
		ptr->SetGameObject(this);
		m_transform = ptr;
		return m_transform;
	}

private:


	std::map<std::type_index, std::shared_ptr<Component>> m_componentMap;
	shared_ptr<Transform> m_transform;
	bool m_destory = false;
};