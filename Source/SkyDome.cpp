﻿#include "main.h"
#include "renderer.h"
#include "model.h"
#include "SkyDome.h"

void SkyDome::Init()
{
	//モデル読み込み
	m_model = new Model();
	m_model->Load((char*)"asset\\model\\skydome.obj");


	Renderer::GetInstance().CreateVertexShader(&m_vertexShader, &m_vertexLayout, "unlitTextureVS.cso");

	Renderer::GetInstance().CreatePixelShader(&m_pixelShader, "unlitTexturePS.cso");

	auto transform = GetComponent<Transform>();
	transform->SetPosition(D3DXVECTOR3(0.0f, -100.0f, 0.0f));
	transform->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	transform->SetScale(D3DXVECTOR3(150.0f, 150.0f, 150.0f));
}

void SkyDome::Uninit()
{
	//m_model->Unload();
	//delete m_model;

	m_vertexLayout->Release();
	m_vertexShader->Release();
	m_pixelShader->Release();
}

void SkyDome::Update()
{
}

void SkyDome::Draw()
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
