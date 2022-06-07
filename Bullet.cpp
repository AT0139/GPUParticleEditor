#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "Scene.h"
#include "Bullet.h"
#include "input.h"

Model* Bullet::m_model;	//スタティックメンバ変数再度宣言


void Bullet::Init()
{
	Renderer::GetInstance()->CreateVertexShader(&m_vertexShader, &m_vertexLayout, "unlitTextureVS.cso");

	Renderer::GetInstance()->CreatePixelShader(&m_pixelShader, "unlitTexturePS.cso");

	m_position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
}

void Bullet::Uninit()
{
	m_vertexLayout->Release();
	m_vertexShader->Release();
	m_pixelShader->Release();
}

void Bullet::Update()
{
	m_position.z += MOVE_SPEED;

	if (m_position.z > 6.0f)
	{
		SetDestroy();
		return;
	}

	Scene* scene = Manager::GetScene();
	//敵当たり判定
	std::vector<Enemy*> enemyList = scene->GetGameObjects<Enemy>(scene->OBJECT);

	for (Enemy* enemy : enemyList)
	{
		D3DXVECTOR3 enemyPosition = enemy->GetPosition();
		D3DXVECTOR3 direction = m_position - enemyPosition;
		float length = D3DXVec3Length(&direction);

		if (length < 2.0f)
		{
			enemy->SetDestroy();
			SetDestroy();
			return;
		}
	}
	//階段当たり判定
}

void Bullet::Draw()
{
	//入力レイアウト設定
	Renderer::GetInstance()->GetDeviceContext()->IASetInputLayout(m_vertexLayout);

	//シェーダー設定
	Renderer::GetInstance()->GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	Renderer::GetInstance()->GetDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

	//ワールドマトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_rotation.x, m_rotation.y, m_rotation.z);
	D3DXMatrixTranslation(&trans, m_position.x, m_position.y, m_position.z);
	world = scale * rot * trans;
	Renderer::GetInstance()->SetWorldMatrix(&world);

	m_model->Draw();
}

void Bullet::Load()
{
	//モデル読み込み
	m_model = new Model();
	m_model->Load((char*)"asset\\model\\torus.obj");
}

void Bullet::Unload()
{
	m_model->Unload();
	delete m_model;
}	
