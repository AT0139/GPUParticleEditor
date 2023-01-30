#include "Polygon2D.h"
#include "Draw2DPolygon.h"
#include "Renderer.h"
#include "ShaderManager.h"


Polygon2D::Polygon2D()
{
	VERTEX_3D vertex[4];

	vertex[0].position = Vector3(0.0f, 0.0f, 0.0f);
	vertex[0].normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[0].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].texCoord = Vector2(0.0f, 0.0f);

	vertex[1].position = Vector3(300.0f, 0.0f, 0.0f);
	vertex[1].normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[1].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = Vector2(1.0f, 0.0f);

	vertex[2].position = Vector3(0.0f, 300.0f, 0.0f);
	vertex[2].normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[2].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = Vector2(0.0f, 1.0f);

	vertex[3].position = Vector3(300.0f, 300.0f, 0.0f);
	vertex[3].normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[3].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].texCoord = Vector2(1.0f, 1.0f);

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
}

Polygon2D::~Polygon2D()
{
	m_vertexBuffer->Release();
}

void Polygon2D::Update()
{
}

void Polygon2D::Draw()
{
	//マトリクス設定
	Renderer::GetInstance().SetWorldViewProjection2D();

	ShaderManager::GetInstance().Set(SHADER_TYPE::UNLIT);

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::GetInstance().SetMaterial(material);

	//Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources(0, 1, &depthTexture);

	Renderer::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	Renderer::GetInstance().GetDeviceContext()->Draw(4, 0);

}