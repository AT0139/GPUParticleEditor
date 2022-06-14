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
#include "Tree.h"

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
		//カメラ
		AddGameObject<Camera>(CAMERA);

		//3Dオブジェクト
		AddGameObject<SkyDome>(OBJECT);
		AddGameObject<Player>(OBJECT);
		AddGameObject<Field>(OBJECT);
		AddGameObject<Stair>(OBJECT)->SetPosition(D3DXVECTOR3(10.0f, 0.0f, 10.0f));
		AddGameObject<Enemy>(OBJECT)->SetPosition(D3DXVECTOR3(-2.0f, 1.0f, 3.0f));
		AddGameObject<Enemy>(OBJECT)->SetPosition(D3DXVECTOR3(0.0f, 1.0f, 3.0f));
		AddGameObject<Enemy>(OBJECT)->SetPosition(D3DXVECTOR3(2.0f, 1.0f, 3.0f));
		AddGameObject<Tree>(OBJECT)->SetPosition(D3DXVECTOR3(-8.0f, 2.0f, 5.0f));

		//2Dオブジェクト
		AddGameObject<Polygon2D>(UI);
	}
	void Uninit()
	{
		for (int i = 0; i < LAYER_MAX; i++)
		{
			for (GameObject* object : m_gameObjects[i])	//範囲for文
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

protected:
	std::list<GameObject*> m_gameObjects[LAYER_MAX];	//STLのリスト構造
};