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
	Scene() {}
	virtual ~Scene() {}

	enum Layer
	{
		CAMERA,
		OBJECT,
		UI,
		LAYER_MAX
	};

	void Init()
	{
		Bullet::Load();
		Enemy::Load();

		//�J����
		AddGameObject<Camera>(CAMERA);

		//3D�I�u�W�F�N�g
		//AddGameObject<SkyDome>();
		AddGameObject<Field>(OBJECT);
		AddGameObject<Stair>(OBJECT);
		AddGameObject<Player>(OBJECT);
		AddGameObject<Enemy>(OBJECT)->SetPosition(D3DXVECTOR3(-2.0f, 1.0f, 3.0f));
		AddGameObject<Enemy>(OBJECT)->SetPosition(D3DXVECTOR3(0.0f, 1.0f, 3.0f));
		AddGameObject<Enemy>(OBJECT)->SetPosition(D3DXVECTOR3(2.0f, 1.0f, 3.0f));

		//2D�I�u�W�F�N�g
		AddGameObject<Polygon2D>(UI);
	}
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
		Bullet::Unload();
		Enemy::Unload();
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