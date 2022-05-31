#pragma once

#include <list>
#include <vector>
#include <typeinfo>
#include "Polygon2D.h"
#include "Field.h"
#include "Camera.h"
#include "model.h"
#include "Player.h"
#include "Stair.h"
#include "Bullet.h"
#include "Enemy.h"
#include "SkyDome.h"

class Scene
{
public:
	Scene() {
	}
	virtual ~Scene() {}


	void Init()
	{
		//�J����
		AddGameObject<Camera>();

		//3D�I�u�W�F�N�g
		//AddGameObject<SkyDome>();
		AddGameObject<Field>();
		AddGameObject<Player>();
		AddGameObject<Stair>();
		AddGameObject<Enemy>()->SetPosition(D3DXVECTOR3(-2.0f, 1.0f, 3.0f));
		AddGameObject<Enemy>()->SetPosition(D3DXVECTOR3(0.0f, 1.0f, 3.0f));
		AddGameObject<Enemy>()->SetPosition(D3DXVECTOR3(2.0f, 1.0f, 3.0f));

		//2D�I�u�W�F�N�g
		//AddGameObject<Polygon2D>();
	}
	void Uninit()
	{
		for (GameObject* object : m_gameObjects)	//�͈�for��
		{
			object->Uninit();
			delete object;
		}
		m_gameObjects.clear();
	}
	void Update()
	{
		for (GameObject* object : m_gameObjects)
		{
			object->Update();
		}

		m_gameObjects.remove_if([](GameObject* object) {return object->Destroy(); });
	}
	void Draw()
	{
		for (GameObject* object : m_gameObjects)
		{
			object->Draw();
		}
	}

	template <typename T>
	T* AddGameObject()
	{
		T* gameobject = new T;
		gameobject->Init();
		m_gameObjects.push_back(gameobject);

		return gameobject;
	}

	template <typename T>
	T* GetGameObject()
	{
		for (GameObject* object : m_gameObjects)	//�^�𒲂ׂ�(RTTI���I�^���)
		{
			if (typeid(*object) == typeid(T))
			{
				return (T*)object;
			}
		}
		return nullptr;
	}

	template <typename T>
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> objects;	//STL�̔z��
		for (GameObject* object : m_gameObjects)	//�^�𒲂ׂ�(RTTI���I�^���)
		{
			if (typeid(*object) == typeid(T))
			{
				objects.push_back((T*)object);
			}
		}
		return objects;
	}

protected:
	std::list<GameObject*> m_gameObjects;	//STL�̃��X�g�\��


};