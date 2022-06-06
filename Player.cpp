#include "main.h"
#include "renderer.h"
#include "model.h"
#include "Scene.h"
#include "manager.h"
#include "Bullet.h"
#include "Player.h"
#include "input.h"

void Player::Init()
{
	//���f���ǂݍ���
	m_model = new Model();
	m_model->Load((char*)"asset\\model\\torus.obj");


	Renderer::GetInstance()->CreateVertexShader(&m_vertexShader, &m_vertexLayout, "unlitTextureVS.cso");

	Renderer::GetInstance()->CreatePixelShader(&m_pixelShader, "unlitTexturePS.cso");

	m_position = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

void Player::Uninit()
{
	m_model->Unload();
	delete m_model;

	m_vertexLayout->Release();
	m_vertexShader->Release();
	m_pixelShader->Release();
}

void Player::Update()
{
	if (Input::GetKeyPress('W'))
		m_position.z += MOVE_SPEED;
	if (Input::GetKeyPress('A'))
		m_position.x -= MOVE_SPEED;
	if (Input::GetKeyPress('S'))
		m_position.z -= MOVE_SPEED;
	if (Input::GetKeyPress('D'))
		m_position.x += MOVE_SPEED;

	if (Input::GetKeyTrigger(VK_SPACE))
	{
		Scene* scene = Manager::GetScene();
		scene->AddGameObject<Bullet>(scene->OBJECT)->SetPosition(m_position);
	}
}

void Player::Draw()
{
	//���̓��C�A�E�g�ݒ�
	Renderer::GetInstance()->GetDeviceContext()->IASetInputLayout(m_vertexLayout);

	//�V�F�[�_�[�ݒ�
	Renderer::GetInstance()->GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	Renderer::GetInstance()->GetDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

	//���[���h�}�g���N�X�ݒ�
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_rotation.x, m_rotation.y, m_rotation.z);
	D3DXMatrixTranslation(&trans, m_position.x, m_position.y, m_position.z);
	world = scale * rot * trans;
	Renderer::GetInstance()->SetWorldMatrix(&world);

	m_model->Draw();
}
