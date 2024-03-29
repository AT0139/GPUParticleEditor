﻿#include "DrawBillboard.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"

DrawBillboard::DrawBillboard(GameObject* pGameObject)
	: Component(pGameObject)
	, m_texture(nullptr)
	, m_count(0)
	, m_isAnimation(false)
	, m_xAdd(1)
	, m_yAdd(1)
{
	VERTEX_3D vertex[4];

	vertex[0].position = Vector3(-1.0f, 1.0f, 0.0f);
	vertex[0].normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[0].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].texCoord = Vector2(0.0f, 0.0f);

	vertex[1].position = Vector3(1.0f, 1.0f, 0.0f);
	vertex[1].normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[1].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = Vector2(1.0f, 0.0f);

	vertex[2].position = Vector3(-1.0f, -1.0f, 0.0f);
	vertex[2].normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[2].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = Vector2(0.0f, 1.0f);

	vertex[3].position = Vector3(1.0f, -1.0f, 0.0f);
	vertex[3].normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[3].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].texCoord = Vector2(1.0f, 1.0f);

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetInstance().GetDevice()->CreateBuffer(&bd, &sd, &m_vertexBuffer);
}

DrawBillboard::~DrawBillboard()
{
	m_vertexBuffer->Release();
}

void DrawBillboard::Update()
{
	if (m_isAnimation)
	{
		m_count++;

		if (m_count >= 16)
		{
			GetGameObject()->SetDestroy();
			return;
		}
	}
}

void DrawBillboard::Draw()
{
	assert(m_texture);

	float x = 0;
	float y = 0;

	//テクスチャ座標産出
	if (m_isAnimation)
	{
		x = m_count % m_xNum * m_xAdd;
		y = m_count / m_xNum * m_yAdd;
	}

	//頂点データ書き換え
	D3D11_MAPPED_SUBRESOURCE msr;
	Renderer::GetInstance().GetDeviceContext()->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

	VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

	vertex[0].position = Vector3(-1.0f, 1.0f, 0.0f);
	vertex[0].normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[0].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].texCoord = Vector2(x, y);

	vertex[1].position = Vector3(1.0f, 1.0f, 0.0f);
	vertex[1].normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[1].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = Vector2(x + m_xAdd, y);

	vertex[2].position = Vector3(-1.0f, -1.0f, 0.0f);
	vertex[2].normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[2].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = Vector2(x, y + m_yAdd);

	vertex[3].position = Vector3(1.0f, -1.0f, 0.0f);
	vertex[3].normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[3].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].texCoord = Vector2(x + m_xAdd, y + m_yAdd);

	Renderer::GetInstance().GetDeviceContext()->Unmap(m_vertexBuffer, 0);

	ShaderManager::GetInstance().Set(SHADER_TYPE::UNLIT);

	//ワールドマトリクス設定
	Matrix world = GetGameObject()->GetComponent<Transform>()->GetWorldMatrixInvView();
	Renderer::GetInstance().SetWorldMatrix(&world);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::GetInstance().SetMaterial(material);

	//テクスチャ設定
	Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources(0, 1, &m_texture);

	//プリミティブトポロジ設定
	Renderer::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetInstance().GetDeviceContext()->Draw(4, 0);
}

void DrawBillboard::Load(const wchar_t* filePath)
{
	m_texture = ResourceManager::GetInstance().GetTextureData(filePath);
}

void DrawBillboard::Load(const wchar_t* filePath, int xNum, int yNum)
{
	m_texture = ResourceManager::GetInstance().GetTextureData(filePath);

	m_isAnimation = true;
	m_xNum = xNum;
	m_yNum = yNum;
	m_xAdd = 1.0f / static_cast<float>(m_xNum);
	m_yAdd = 1.0f / static_cast<float>(m_yNum);
}