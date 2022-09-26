#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "Camera.h"
#include "input.h"
#include "Scene.h"
#include "Player.h"

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
	//�v���C���[�̎擾
	Scene* scene = Manager::GetInstance()->GetScene();
	Player* player = scene->GetGameObject<Player>(scene->OBJECT);
	D3DXVECTOR3 playerPos = player->GetPosition();
	D3DXVECTOR3 playerForward = player->GetForward();
	D3DXVECTOR3 playerRight = player->GetRight();

	//�g�b�v�r���[
	//m_target = playerPos;
	//m_position = playerPos + D3DXVECTOR3(0.0f, 5.0f, -5.0f);


	//�T�[�h�p�[�\���r���[
	m_target = playerPos;
	m_position = playerPos + playerForward * 5.0f + D3DXVECTOR3(0.0f, 2.5f, 0.0f);

	//�T�[�h�p�[�\���r���[(�E���)
	//m_target = playerPos + playerRight;
	//m_position = playerPos + playerForward * 5.0f + playerRight + D3DXVECTOR3(0.0f, 2.0f, 0.0f);

	////�t�@�[�X�g�p�[�\���r���[
	//m_target = playerPos + playerForward;
	//m_position = playerPos;
}

void Camera::Draw()
{
	//�r���[�}�g���N�X�ݒ�
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &m_target, &up);

	Renderer::GetInstance()->SetViewMatrix(&m_viewMatrix);

	//�v���W�F�N�V�����}�g���N�X�ݒ�	
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::GetInstance()->SetProjectionMatrix(&projectionMatrix);
}
