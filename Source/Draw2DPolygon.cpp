#include "Draw2DPolygon.h"
#include "Renderer.h"
#include "ResourceManager.h"

Draw2DPolygon::Draw2DPolygon(GameObject* gameObject)
	: Component(gameObject)
	, m_vertexBuffer(nullptr)
	, m_texture(nullptr)
	, m_vertexShader(nullptr)
	, m_pixelShader(nullptr)
	, m_vertexLayout(nullptr)
	, m_position(Vector2(0.0f, 0.0f))
	, m_size(Vector2(100.0f, 100.0f))
	, m_uv(0.0f, 0.0f)
	, m_uvSize(1.0f, 1.0f)
	, m_alpha(1.0f)
{
	VERTEX_3D vertex[4];

	vertex[0].position = Vector3(0.0f, 0.0f, 0.0f);
	vertex[0].normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[0].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].texCoord = Vector2(0.0f, 0.0f);

	vertex[1].position = Vector3(100.0f, 0.0f, 0.0f);
	vertex[1].normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[1].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = Vector2(1.0f, 0.0f);

	vertex[2].position = Vector3(0.0f, 100.0f, 0.0f);
	vertex[2].normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[2].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = Vector2(0.0f, 1.0f);

	vertex[3].position = Vector3(100.0f, 100.0f, 0.0f);
	vertex[3].normal = Vector3(0.0f, 0.0f, 0.0f);
	vertex[3].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].texCoord = Vector2(1.0f, 1.0f);

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;
	Renderer::GetInstance().GetDevice()->CreateBuffer(&bd, &sd, &m_vertexBuffer);


	Renderer::GetInstance().CreateVertexShader(&m_vertexShader, &m_vertexLayout, "unlitTextureVS.cso");
	Renderer::GetInstance().CreatePixelShader(&m_pixelShader, "unlitTexturePS.cso");
}

Draw2DPolygon::~Draw2DPolygon()
{
	m_vertexBuffer->Release();
	m_texture->Release();

	m_vertexLayout->Release();
	m_vertexShader->Release();
	m_pixelShader->Release();
}

void Draw2DPolygon::Update()
{
}

void Draw2DPolygon::Draw()
{
	//頂点データの書き換え
	{
		D3D11_MAPPED_SUBRESOURCE msr;
		Renderer::GetInstance().GetDeviceContext()->Map(m_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);

		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		vertex[0].position = Vector3(m_position.x - m_size.x * 0.5f, m_position.y - m_size.y * 0.5f, 0.0f);
		vertex[0].normal = Vector3(0.0f, 0.0f, 0.0f);
		vertex[0].diffuse = Vector4(1.0f, 1.0f, 1.0f, m_alpha);
		vertex[0].texCoord = Vector2(m_uv.x, m_uv.y);

		vertex[1].position = Vector3(m_position.x + m_size.x * 0.5f, m_position.y - m_size.y * 0.5f, 0.0f);
		vertex[1].normal = Vector3(0.0f, 0.0f, 0.0f);
		vertex[1].diffuse = Vector4(1.0f, 1.0f, 1.0f, m_alpha);
		vertex[1].texCoord = Vector2(m_uv.x + m_uvSize.x, m_uv.y);

		vertex[2].position = Vector3(m_position.x - m_size.x * 0.5f, m_position.y + m_size.y * 0.5f, 0.0f);
		vertex[2].normal = Vector3(0.0f, 0.0f, 0.0f);
		vertex[2].diffuse = Vector4(1.0f, 1.0f, 1.0f, m_alpha);
		vertex[2].texCoord = Vector2(m_uv.x, m_uv.y + m_uvSize.y);

		vertex[3].position = Vector3(m_position.x + m_size.x * 0.5f, m_position.y + m_size.y * 0.5f, 0.0f);
		vertex[3].normal = Vector3(0.0f, 0.0f, 0.0f);
		vertex[3].diffuse = Vector4(1.0f, 1.0f, 1.0f, m_alpha);
		vertex[3].texCoord = Vector2(m_uv.x + m_uvSize.x, m_uv.y + m_uvSize.y);

		Renderer::GetInstance().GetDeviceContext()->Unmap(m_vertexBuffer, 0);
	}
	//入力レイアウト設定
	Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_vertexLayout);

	//シェーダー設定
	Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

	//マトリクス設定
	Renderer::GetInstance().SetWorldViewProjection2D();

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//テクスチャ設定
	Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources(0, 1, &m_texture);

	//プリミティブトポロジ設定
	Renderer::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//ポリゴン描画
	Renderer::GetInstance().GetDeviceContext()->Draw(4, 0);
}

void Draw2DPolygon::SetPosition(Vector2 pos)
{
	m_position = pos;
}

void Draw2DPolygon::SetSize(Vector2 size)
{
	m_size = size;
}

void Draw2DPolygon::SetUV(Vector2 uv)
{
	m_uv = uv;
}

void Draw2DPolygon::SetUV(Vector2 uv, Vector2 size)
{
	m_uv = uv;
	m_uvSize = size;
}

void Draw2DPolygon::SetAlpha(float alpha)
{
	m_alpha = alpha;
}

void Draw2DPolygon::LoadTexture(std::string fileName)
{
	m_texture = ResourceManager::GetInstance().GetTextureData(fileName);
}
