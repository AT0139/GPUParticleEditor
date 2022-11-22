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


namespace MainGame
{
	Model* Bullet::m_model;	//スタティックメンバ変数再度宣言

	void Bullet::Init()
	{
		m_model = ResourceManager::GetInstance().GetModelData("asset\\model\\torus.obj");

		Renderer::GetInstance().CreateVertexShader(&m_vertexShader, &m_vertexLayout, "unlitTextureVS.cso");

		Renderer::GetInstance().CreatePixelShader(&m_pixelShader, "unlitTexturePS.cso");

		auto transform = GetComponent<Transform>();
		transform->SetPosition(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		transform->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		transform->SetScale(D3DXVECTOR3(0.2f, 0.2f, 0.2f));

		Player* player = Manager::GetInstance().GetScene()->GetGameObject<Player>(Manager::GetInstance().GetScene()->OBJECT);
		m_forward = transform->GetForward();
	}

	void Bullet::Uninit()
	{
		m_vertexLayout->Release();
		m_vertexShader->Release();
		m_pixelShader->Release();
	}

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

	void Bullet::Draw()
	{
		//入力レイアウト設定
		Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_vertexLayout);

		//シェーダー設定
		Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
		Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

		//ワールドマトリクス設定
		auto transform = GetComponent<Transform>();
		auto world = transform->GetWorldMatrix();
		Renderer::GetInstance().SetWorldMatrix(&world);

		m_model->Draw();
	}

	void Bullet::Load()
	{
		//モデル読み込み
		m_model = ResourceManager::GetInstance().GetModelData("asset\\model\\torus.obj");
	}

	void Bullet::Unload()
	{
		//m_model->Unload();
		//delete m_model;
	}
}