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
		AddGameObject<Camera>(0);

		//3D�I�u�W�F�N�g
		//AddGameObject<SkyDome>();
		AddGameObject<Field>(1);
		AddGameObject<Stair>(1);
		AddGameObject<Player>(1);
		AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(-2.0f, 1.0f, 3.0f));
		AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(0.0f, 1.0f, 3.0f));
		AddGameObject<Enemy>(1)->SetPosition(D3DXVECTOR3(2.0f, 1.0f, 3.0f));

		//2D�I�u�W�F�N�g
		AddGameObject<Polygon2D>(2);
	}
	void Uninit()
	{
		for (int i = 0; i < 3; i++)
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

		for (int i = 0; i < 3; i++)
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
		for (int i = 0; i < 3; i++)
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
	std::list<GameObject*> m_gameObjects[3];	//STL�̃��X�g�\��
};