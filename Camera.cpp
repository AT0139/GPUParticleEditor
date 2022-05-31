#include "main.h"
#include "renderer.h"
#include "Camera.h"

void Camera::Init()
{
	m_position = D3DXVECTOR3(0.0f, 5.0f, -5.0f);
	m_target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void Camera::Uninit()
{
}

void Camera::Update()
{
}

void Camera::Draw()
{
	//ビューマトリクス設定
	D3DXMATRIX viewMatrix;
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&viewMatrix, &m_position, &m_target, &up);

	Renderer::GetInstance()->SetViewMatrix(&viewMatrix);

	//プロジェクションマトリクス設定	
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::GetInstance()->SetProjectionMatrix(&projectionMatrix);
}
