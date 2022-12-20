#include "Renderer.h"
#include "DrawModel.h"
#include "Model.h"
#include "ResourceManager.h"
#include "GameObject.h"

DrawModel::DrawModel(GameObject* pGameObject)
	: Component(pGameObject)
	, m_model(nullptr)
	, m_rasterizerState(RASTERIZER::DEFAULT)
	, m_isCollision(false)
{
	Renderer::GetInstance().CreateVertexShader(&m_vertexShader, &m_vertexLayout, "unlitTextureVS.cso");

	Renderer::GetInstance().CreatePixelShader(&m_pixelShader, "unlitTexturePS.cso");
}

DrawModel::~DrawModel()
{
	m_vertexLayout->Release();
	m_vertexShader->Release();
	m_pixelShader->Release();
}

void DrawModel::Update()
{
}

void DrawModel::Draw()
{
	assert(m_model);

	if (m_rasterizerState == RASTERIZER::WIRE_FRAME)
		Renderer::GetInstance().SetRasterizerState(RASTERIZER::WIRE_FRAME);

	//入力レイアウト設定
	Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_vertexLayout);

	//シェーダー設定
	Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

	//ワールドマトリクス設定
	Matrix world;
	if (m_isCollision)
	{
		auto scale = GetGameObject()->GetComponent<CollisionComponent>()->GetCollisionScale();
		world = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix(scale);
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

void DrawModel::SetVertexShader(const char* filename)
{
	m_vertexLayout->Release();
	m_vertexShader->Release();
	Renderer::GetInstance().CreateVertexShader(&m_vertexShader, &m_vertexLayout, filename);
}

void DrawModel::SetPixelShader(const char* filename)
{
	m_pixelShader->Release();
	Renderer::GetInstance().CreatePixelShader(&m_pixelShader, filename);
}
