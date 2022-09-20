#include "main.h"
#include "renderer.h"
#include "AnimationModel.h"
#include "Scene.h"
#include "manager.h"
#include "Bullet.h"
#include "audio.h"
#include "Shadow.h"
#include "Player.h"
#include "input.h"


void Player::Init()
{
	//モデル読み込み
	m_model = new AnimationModel();
	m_model->Load((char*)"asset\\model\\Akai_Idle.fbx");
	m_model->LoadAnimation((char*)"asset\\model\\Akai_Idle.fbx", "Idol");
	m_model->LoadAnimation((char*)"asset\\model\\Akai_FastRun.fbx","Run");

	m_animationName = "Idol";

	Renderer::GetInstance()->CreateVertexShader(&m_vertexShader, &m_vertexLayout, "unlitTextureVS.cso");

	Renderer::GetInstance()->CreatePixelShader(&m_pixelShader, "unlitTexturePS.cso");

	Scene* scene = Manager::GetInstance()->GetScene();
	//m_shotSE = scene->AddGameObject<Audio>(scene->UI);
	//m_shotSE->Load("asset\\audio\\wan.wav");

	//m_shadow = scene->AddGameObject<Shadow>(scene->OBJECT);
	//m_shadow->SetPosition(m_position);
	//m_shadow->SetScale(D3DXVECTOR3(2.0f, 1.0f, 2.0f));

	m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(0.008f, 0.008f, 0.008f);
	//m_scale = D3DXVECTOR3(0.3f, 0.3f, 0.3f);
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
	m_model->Update(m_animationName.c_str(), m_frame);

	//ROTATION
	if (Input::GetKeyPress('Q'))
		m_rotation.y -= 0.1f;
	if (Input::GetKeyPress('E'))
		m_rotation.y += 0.1f;

	D3DXVECTOR3 forward = GetForward();
	D3DXVECTOR3 right = GetRight();

	//MOVE
	if (Input::GetKeyPress('W'))
	{
		m_position += forward * MOVE_SPEED;
		m_animationName = "Run";
	}
	else if (Input::GetKeyPress('S'))
	{
		m_position -= forward * MOVE_SPEED;
		m_animationName = "Run";
	}
	else
		m_animationName = "Idol";

	if (Input::GetKeyPress('A'))
		m_position -= right * MOVE_SPEED;
	if (Input::GetKeyPress('D'))
		m_position += right * MOVE_SPEED;

	//SHOT
	if (Input::GetKeyTrigger(VK_SPACE))
	{
		Scene* scene = Manager::GetScene();
		scene->AddGameObject<Bullet>(scene->OBJECT)->SetPosition(m_position);

		//m_shotSE->Play();
	}

	//D3DXVECTOR3 shadowPos = m_position;
	//shadowPos.y = 0.01f;
	//m_shadow->SetPosition(shadowPos);

	m_frame++;

}

void Player::Draw()
{
	//入力レイアウト設定
	Renderer::GetInstance()->GetDeviceContext()->IASetInputLayout(m_vertexLayout);

	//シェーダー設定
	Renderer::GetInstance()->GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	Renderer::GetInstance()->GetDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

	//ワールドマトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_rotation.y, m_rotation.x, m_rotation.z);
	D3DXMatrixTranslation(&trans, m_position.x, m_position.y, m_position.z);
	world = scale * rot * trans;
	Renderer::GetInstance()->SetWorldMatrix(&world);

	m_model->Draw();
}
