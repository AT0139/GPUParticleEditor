#pragma once

#include <list>
#include "Polygon2D.h"
#include "Field.h"
#include "Camera.h"
#include "model.h"
#include "Player.h"
#include "Stair.h"
#include "Enemy.h"
#include "Bullet.h"

class Scene
{
public:
	Scene() {
	}
	virtual ~Scene() {}


	void Init()
	{
		//カメラ
		AddGameObject<Camera>();

		//3Dオブジェクト
		AddGameObject<Field>();
		AddGameObject<Player>();
		AddGameObject<Stair>();
		AddGameObject<Enemy>()->SetPosition(D3DXVECTOR3(-2.0f, 1.0f, 3.0f));
		AddGameObject<Enemy>()->SetPosition(D3DXVECTOR3(0.0f, 1.0f, 3.0f));
		AddGameObject<Enemy>()->SetPosition(D3DXVECTOR3(2.0f, 1.0f, 3.0f));
		AddGameObject<Bullet>();

		//2Dオブジェクト
		//AddGameObject<Polygon2D>();
	}
	void Uninit()
	{
		for (GameObject* object : m_gameObjects)	//範囲for文
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

protected:
	std::list<GameObject*> m_gameObjects;	//STLのリスト構造


};