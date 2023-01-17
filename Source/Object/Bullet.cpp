#include "Bullet.h"
#include "DrawBillboard.h"


namespace MainGame
{
	Bullet::Bullet()
	{
		AddComponent<DrawBillboard>(this)->Load(L"Asset\\Texture\\Bullet.png");

		auto rigid = AddComponent<Rigidbody>();
		rigid->SetIsTrigger(true);
		rigid->SetGravityZero();

		auto col = AddComponent<SphereCollision>();
		col->SetScale(0.3f);

		SetTag(TAG::BULLET);
	}

	Bullet::~Bullet()
	{}

	void Bullet::Update()
	{
	}

	void Bullet::SetVelocity(Vector3 velocity)
	{
		GetComponent<Rigidbody>()->SetVelocity(velocity);
	}
}