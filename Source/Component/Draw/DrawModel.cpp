#include "Renderer.h"
#include "CModel.h"
#include "DrawModel.h"
#include "ResourceManager.h"
#include "GameObject.h"

DrawModel::DrawModel(GameObject* pGameObject)
	: Component(pGameObject)
	, m_model(nullptr)
	, m_rasterizerState(RASTERIZER::DEFAULT)
	, m_isCollision(false)
	, m_shaderType(SHADER_TYPE::PIXEL_LIGHTING)
{
}

DrawModel::~DrawModel()
{
}

void DrawModel::Update()
{
}

void DrawModel::Draw()
{
	assert(m_model);

	if (m_rasterizerState == RASTERIZER::WIRE_FRAME)
		Renderer::GetInstance().SetRasterizerState(RASTERIZER::WIRE_FRAME);

	////入力レイアウト設定
	//Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_vertexLayout);

	////シェーダー設定
	//Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	//Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

	ShaderManager::GetInstance().Set(m_shaderType);

	//ワールドマトリクス設定
	Matrix world;
	if (m_isCollision)
	{
		auto scale = GetGameObject()->GetComponent<CollisionComponent>()->GetCollisionScale();
		auto position = GetGameObject()->GetComponent<CollisionComponent>()->GetCenterPosition();
		world = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix(scale, position);
	}
	else
		world = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix();

	Renderer::GetInstance().SetWorldMatrix(&world);

	m_model->Draw();

	if (m_rasterizerState == RASTERIZER::WIRE_FRAME)
		Renderer::GetInstance().SetRasterizerState(RASTERIZER::DEFAULT);
}

void DrawModel::Load(const char* filePath)
{
	m_model = ResourceManager::GetInstance().GetModelData(filePath);
}

void DrawModel::SetShader(SHADER_TYPE type)
{
	m_shaderType = type;
}