#pragma once

#include <list>
#include <vector>
#include <typeinfo>
#include "main.h"
#include "GameObject.h"

class Scene
{
public:
	Scene() {}
	virtual ~Scene() {}

	enum Layer
	{
		CAMERA,
		OBJECT,
		UI,
		LAYER_MAX
	};

	virtual void Init() = 0;	//�������z�֐�

	void Uninit()
	{
		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (GameObject* object : m_gameObjects[i])	//�͈�for��
			{
				object->Uninit();
				delete object;
			}
			m_gameObjects[i].clear();
		}
	}
	void Update()
	{
		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (GameObject* object : m_gameObjects[i])
			{
				object->Update();
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
				object->Draw();
			}
		}
	}

	template <typename T>
	T* AddGameObject(int layer)
	{
		T* gameobject = new T;
		m_gameObjects[layer].push_back(gameobject);
		gameobject->Init();

		return gameobject;
	}

	template <typename T>
	T* GetGameObject(int layer)
	{
		for (GameObject* object : m_gameObjects[layer])	//�^�𒲂ׂ�(RTTI���I�^���)
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
		std::vector<T*> objects;	//STL�̔z��
		for (GameObject* object : m_gameObjects[layer])	//�^�𒲂ׂ�(RTTI���I�^���)
		{
			if (typeid(*object) == typeid(T))
			{
				objects.push_back((T*)object);
			}
		}
		return objects;
	}

protected:
	std::list<GameObject*> m_gameObjects[LAYER_MAX];	//STL�̃��X�g�\��
};