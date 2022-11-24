#include "DrawModel.h"
#include "Renderer.h"
#include "Model.h"
#include "ResourceManager.h"
#include "GameObject.h"

DrawModel::DrawModel(GameObject* pGameObject)
	: Component(pGameObject)
	, m_model(nullptr)
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

	//入力レイアウト設定
	Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_vertexLayout);

	//シェーダー設定
	Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

	////ワールドマトリクス設定
	D3DXMATRIX world = GetGameObject()->GetComponent<Transform>()->GetWorldMatrix();
	Renderer::GetInstance().SetWorldMatrix(&world);

	m_model->Draw();
}

void DrawModel::Load(const char* filePath)
{
	m_model = ResourceManager::GetInstance().GetModelData(filePath);
}