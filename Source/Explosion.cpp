#include "main.h"
#include "renderer.h"
#include "Explosion.h"
#include "manager.h"
#include "Scene.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "DrawBillboard.h"

namespace MainGame
{
	Explosion::Explosion()
	{
		AddComponent<DrawBillboard>(this)->Load("asset/texture/explosion.png", 4, 4);

		auto transform = GetComponent<Transform>();

		transform->SetPosition(Vector3(0.0f, 0.0f, 0.0f));
		transform->SetQuaternion(Quaternion(0.0f, 0.0f, 0.0f, 0.0f));
		transform->SetScale(Vector3(1.0f, 1.0f, 1.0f));
	}

	Explosion::~Explosion()
	{
	}

	void Explosion::Update()
	{
	}

	void Explosion::Draw()
	{}
}