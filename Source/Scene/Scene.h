#pragma once

#include <list>
#include <vector>
#include <typeinfo>
#include "main.h"
#include "GameObject.h"
#include "Camera.h"

class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}

	enum Layer
	{
		CAMERA,
		OBJECT,
		EFFECT,

		UI,
		LAYER_MAX
	};

	virtual void Init() = 0;	//純粋仮想関数

	virtual void Uninit()
	{
		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (GameObject* object : m_gameObjects[i])	//範囲for文
			{
				delete object;
			}
			m_gameObjects[i].clear();
		}
	}
	virtual void Update()
	{
		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (GameObject* object : m_gameObjects[i])
			{
				if (!object->IsHidden())
				{
					object->Update();
					object->ComponentUpdate();
					object->TransformUpdate();
					object->RigidbodyUpdate();
				}
			}

			m_gameObjects[i].remove_if([](GameObject* object) {return object->Destroy(); });
		}
	}
	void Draw()
	{
		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (GameObject* object : m_gameObjects[i])
			{
				if (!object->IsHidden())
				{
					object->Draw();
					object->ComponentDraw();
				}
			}
		}
	}

	template <typename T>
	T* AddGameObject(int layer)
	{
		T* gameobject = new T;
		m_gameObjects[layer].push_back(gameobject);

		return gameobject;
	}

	template <typename T>
	T* GetGameObject(int layer)
	{
		for (GameObject* object : m_gameObjects[layer])	//型を調べる(RTTI動的型情報)
		{
			if (typeid(*object) == typeid(T))
			{
				return (T*)object;
			}
		}
		return nullptr;
	}

	template <typename T>
	std::vector<T*> GetGameObjects(int layer)
	{
		std::vector<T*> objects;	//STLの配列
		for (GameObject* object : m_gameObjects[layer])	//型を調べる(RTTI動的型情報)
		{
			if (typeid(*object) == typeid(T))
			{
				objects.push_back((T*)object);
			}
		}
		return objects;
	}

	std::list<GameObject*>& GetAllObject()
	{
		return m_gameObjects[OBJECT];
	}

	template <typename T>
	void SetCamera()
	{
		T* gameobject = new T;
		m_gameObjects[CAMERA].push_back(gameobject);
		m_currentCamera = gameobject;
	}

	Camera* GetCamera()
	{
		return m_currentCamera;
	}

protected:
	std::list<GameObject*> m_gameObjects[LAYER_MAX];	//STLのリスト構造
	Camera* m_currentCamera;
};