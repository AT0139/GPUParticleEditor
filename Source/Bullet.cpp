#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Enemy.h"
#include "Bullet.h"
#include "input.h"
#include "Explosion.h"
#include "Player.h"
#include "DrawModel.h"

namespace MainGame
{
	Bullet::Bullet()
	{
		AddComponent<DrawModel>(this)->Load("asset\\model\\torus.obj");

		auto transform = GetComponent<Transform>();
		transform->SetPosition(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		transform->SetRotation(D3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f));
		transform->SetScale(D3DXVECTOR3(0.2f, 0.2f, 0.2f));

		Player* player = Manager::GetInstance().GetScene()->GetGameObject<Player>(Manager::GetInstance().GetScene()->OBJECT);
		m_forward = transform->GetForward();
	}

	Bullet::~Bullet()
	{}

	void Bullet::Update()
	{
		Scene* scene = Manager::GetInstance().GetScene();
		auto transform = GetComponent<Transform>();

		transform->SetPosition(m_forward * MOVE_SPEED);

		if (m_count > DESTROY_COUNT)
		{
			SetDestroy();
			return;
		}

		//敵当たり判定
		std::vector<Enemy*> enemyList = scene->GetGameObjects<Enemy>(scene->OBJECT);

		for (Enemy* enemy : enemyList)
		{
			D3DXVECTOR3 enemyPosition = enemy->GetComponent<Transform>()->GetPosition();
			D3DXVECTOR3 direction = transform->GetPosition() - enemyPosition;
			float length = D3DXVec3Length(&direction);

			if (length < 2.0f)
			{
				enemy->SetDestroy();
				SetDestroy();
				scene->AddGameObject<Explosion>(scene->OBJECT)->GetComponent<Transform>()->SetPosition(enemyPosition);
				return;
			}
		}

		m_count++;
	}
}