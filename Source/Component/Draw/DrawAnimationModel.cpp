#include "DrawAnimationModel.h"
#include "AnimationModel.h"
#include "Renderer.h"
#include "GameObject.h"
#include "ResourceManager.h"

static const float ADD_BLENDRATE = 0.05f;


DrawAnimationModel::DrawAnimationModel(GameObject* pGameObject)
	: Component(pGameObject)
	, m_model(nullptr)
	, m_blendRate(0.0f)
	, m_frame(0)
	, m_animationName("Idle")
{
	Renderer::GetInstance().CreateVertexShader(&m_vertexShader, &m_vertexLayout, "vertexLightingVS.cso");
	Renderer::GetInstance().CreatePixelShader(&m_pixelShader, "vertexLightingPS.cso");
}

DrawAnimationModel::~DrawAnimationModel()
{
	m_vertexLayout->Release();
	m_vertexShader->Release();
	m_pixelShader->Release();
}

void DrawAnimationModel::Update()
{
	if (m_blendRate > 1.0f)
		m_blendRate = 1.0f;
	if (m_blendRate < 0.0f)
		m_blendRate = 0.0f;

	m_model->Update(m_animationName.c_str(), m_blendRate, m_frame);
	m_frame++;

	if (m_model->IsResetBlendRate())
	{
		m_blendRate = 0.0f;
		m_model->FalseResetBlendRate();
	}
}


void DrawAnimationModel::Draw()
{
	//入力レイアウト設定
	Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_vertexLayout);

	//シェーダー設定
	Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

	//ワールドマトリクス設定
	Matrix world = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix();
	Renderer::GetInstance().SetWorldMatrix(&world);

	m_model->Draw();
}

void DrawAnimationModel::Load(std::string filePath)
{
	m_model = ResourceManager::GetInstance().GetAnimationModelData(filePath);
	m_model->LoadAnimation(filePath.c_str(), "Idle");
}

void DrawAnimationModel::LoadAnimation(const char* fileName, const char* animarionName)
{
	m_model->LoadAnimation(fileName, animarionName);

}

void DrawAnimationModel::SetAnimationName(std::string animationName)
{
	m_animationName = animationName;
}


void DrawAnimationModel::SetVertexShader(const char* filename)
{
	m_vertexLayout->Release();
	m_vertexShader->Release();
	Renderer::GetInstance().CreateVertexShader(&m_vertexShader, &m_vertexLayout, filename);
}

void DrawAnimationModel::SetPixelShader(const char* filename)
{
	m_pixelShader->Release();
	Renderer::GetInstance().CreatePixelShader(&m_pixelShader, filename);
}

void DrawAnimationModel::AddBlendRate()
{
	m_blendRate += ADD_BLENDRATE;
}

void DrawAnimationModel::AddBlendRate(float factor)
{
	m_blendRate += ADD_BLENDRATE * factor;
}
