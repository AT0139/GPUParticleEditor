#include "main.h"
#include "renderer.h"
#include "model.h"
#include "ModelManager.h"
#include "Enemy.h"


Model* Enemy::m_model;

void Enemy::Init()
{
	Renderer::GetInstance()->CreateVertexShader(&m_vertexShader, &m_vertexLayout, "unlitTextureVS.cso");
	Renderer::GetInstance()->CreatePixelShader(&m_pixelShader, "unlitTexturePS.cso");

	m_position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rotation = D3DXVECTOR3(0.0f, 90.0f, 0.0f);
	m_scale = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
}

void Enemy::Uninit()
{
	m_vertexLayout->Release();
	m_vertexShader->Release();
	m_pixelShader->Release();
}

void Enemy::Update()
{
}

void Enemy::Draw()
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

void Enemy::Load()
{	
	//モデル読み込み
	m_model = ModelManager::GetInstance()->GetModelData("asset\\model\\torus.obj");
};


void Enemy::Unload()
{
	m_model->Unload();
	delete m_model;
}
