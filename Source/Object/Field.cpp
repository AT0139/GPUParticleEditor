#include "main.h"
#include "renderer.h"
#include "Field.h"
#include "ResourceManager.h"

Field::Field()
{
	VERTEX_3D vertex[4];

	vertex[0].position = Vector3(-10.0f, 0.0f, 10.0f);
	vertex[0].normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[0].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].texCoord = Vector2(0.0f, 0.0f);

	vertex[1].position = Vector3(10.0f, 0.0f, 10.0f);
	vertex[1].normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[1].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = Vector2(10.0f, 0.0f);

	vertex[2].position = Vector3(-10.0f, 0.0f, -10.0f);
	vertex[2].normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[2].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = Vector2(0.0f, 10.0f);

	vertex[3].position = Vector3(10.0f, 0.0f, -10.0f);
	vertex[3].normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[3].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].texCoord = Vector2(10.0f, 10.0f);

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetInstance().GetDevice()->CreateBuffer(&bd, &sd, &m_vertexBuffer);

	//テクスチャ読み込み
	m_texture = ResourceManager::GetInstance().GetTextureData(L"asset/texture/field000.jpg");
	assert(m_texture);

	auto transform = GetComponent<Transform>();
	transform->SetQuaternion(Quaternion(0.0f, 0.0f, 0.0f, 0.0f));
	transform->SetScale(Vector3(1.0f, 1.0f, 1.0f));
}

Field::~Field()
{
	m_vertexBuffer->Release();
	m_texture->Release();
}

void Field::Update()
{
}

void Field::Draw()
{
	ShaderManager::GetInstance().Set(SHADER_TYPE::DEPTH);

	//ワールドマトリクス設定
	Matrix world = GetComponent<Transform>()->GetWorldMatrix();
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
	auto depthTexture = Renderer::GetInstance().GetShadowDepthTexture();
	Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources(1, 1, &depthTexture);

	//プリミティブトポロジ設定
	Renderer::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetInstance().GetDeviceContext()->Draw(4, 0);
}