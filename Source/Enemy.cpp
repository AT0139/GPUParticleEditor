#include "main.h"
#include "renderer.h"
#include "model.h"
#include "ResourceManager.h"
#include "Enemy.h"


Model* Enemy::m_model;

void Enemy::Init()
{
	m_model = ResourceManager::GetInstance().GetModelData("asset\\model\\torus.obj");

	Renderer::GetInstance().CreateVertexShader(&m_vertexShader, &m_vertexLayout, "unlitTextureVS.cso");
	Renderer::GetInstance().CreatePixelShader(&m_pixelShader, "unlitTexturePS.cso");

	auto transform = GetComponent<Transform>();
	transform->SetPosition(D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	transform->SetRotation(D3DXVECTOR3(0.0f, 90.0f, 0.0f));
	transform->SetScale(D3DXVECTOR3(0.5f, 0.5f, 0.5f));
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
	Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_vertexLayout);

	//シェーダー設定
	Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);
	
	//ワールドマトリクス設定
	D3DXMATRIX world = GetComponent<Transform>()->GetWorldMatrix();
	Renderer::GetInstance().SetWorldMatrix(&world);

	m_model->Draw();
}

void Enemy::Load()
{	
	//モデル読み込み
	m_model = ResourceManager::GetInstance().GetModelData("asset\\model\\torus.obj");
};


void Enemy::Unload()
{
	//m_model->Unload();
	//delete m_model;
}
