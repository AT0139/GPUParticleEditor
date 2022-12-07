#pragma once

#include "Component.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "CollisionComponent.h"
#include "SphereCollision.h"
#include "AABBCollision.h"
#include "OBBCollision.h"

using std::shared_ptr;

enum class TAG
{
	NONE,
	PLAYER,
	ENEMY,
};

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
		auto ptr = SearchComponent(std::type_index(typeid(T)));
		if (ptr)
		{
			auto pRet = std::dynamic_pointer_cast<T>(ptr);
			if (pRet)
				return pRet;
		}
		return nullptr;
	}

	template<>
	shared_ptr<Transform> GetComponent<Transform>()const
	{
		assert(m_transform);
		return m_transform;
	}

	template<>
	shared_ptr<Rigidbody> GetComponent<Rigidbody>()const
	{
		assert(m_rigidbody);
		return m_rigidbody;
	}

	template<>
	shared_ptr<CollisionComponent> GetComponent<CollisionComponent>()const
	{
		if (!m_collision)
			return nullptr;

		return m_collision;
	}

	template<typename T, typename... U>
	shared_ptr<T> AddComponent(U&&... param)
	{
		std::shared_ptr<T> ptr = shared_ptr<T>(new T(param...));
		m_componentList.push_back(ptr);
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

	template<>
	shared_ptr<Rigidbody> AddComponent<Rigidbody>()
	{
		std::shared_ptr<Rigidbody> ptr(new Rigidbody(this));
		ptr->SetGameObject(this);
		m_rigidbody = ptr;
		return m_rigidbody;
	}

	template<>
	shared_ptr<SphereCollision> AddComponent<SphereCollision>()
	{
		std::shared_ptr<SphereCollision> ptr(new SphereCollision(this));
		ptr->SetGameObject(this);
		m_collision = ptr;
		return ptr;
	}

	template<>
	shared_ptr<AABBCollision> AddComponent<AABBCollision>()
	{
		std::shared_ptr<AABBCollision> ptr(new AABBCollision(this));
		ptr->SetGameObject(this);
		m_collision = ptr;
		return ptr;
	}

	template<>
	shared_ptr<OBBCollision> AddComponent<OBBCollision>()
	{
		std::shared_ptr<OBBCollision> ptr(new OBBCollision(this));
		ptr->SetGameObject(this);
		m_collision = ptr;
		return ptr;
	}

	void ComponentUpdate()
	{
		for (auto comp : m_componentList)
		{
			comp->Update();
		}
	}
	void ComponentDraw()
	{
		for (auto comp : m_componentList)
		{
			comp->Draw();
		}
	}

	void TransformUpdate()
	{
		m_transform->Update();
	}
	void RigidbodyUpdate()
	{
		if (m_rigidbody)
			m_rigidbody->Update();
	}

	virtual void OnCollision(GameObject* collision) {};

	TAG GetTag() { return m_tag; }

protected:
	TAG m_tag;

private:
	template<typename T>
	shared_ptr<Component> SearchComponent(std::type_index index)const
	{
		for (auto comp : m_componentList)
		{
			if (typeid(comp) == typeid(T))
			{
				return comp;
			}
		}
		return nullptr;
	}

	std::list<std::shared_ptr<Component>> m_componentList;

	shared_ptr<Transform> m_transform;
	shared_ptr<CollisionComponent> m_collision;
	shared_ptr<Rigidbody> m_rigidbody;
	bool m_destory = false;
};