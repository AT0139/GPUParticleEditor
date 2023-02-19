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
	, m_shaderType(SHADER_TYPE::VERTEX_LIGHTING)
{
}

DrawAnimationModel::~DrawAnimationModel()
{
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
	ShaderManager::GetInstance().Set(m_shaderType);

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

void DrawAnimationModel::SetShader(SHADER_TYPE type)
{
	m_shaderType = type;
}

void DrawAnimationModel::AddBlendRate()
{
	m_blendRate += ADD_BLENDRATE;
}

void DrawAnimationModel::AddBlendRate(float factor)
{
	m_blendRate += ADD_BLENDRATE * factor;
}
