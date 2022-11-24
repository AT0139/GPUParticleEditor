#include "main.h"
#include "renderer.h"
#include "Tree.h"
#include "manager.h"
#include "Scene.h"
#include "Camera.h"
#include "ResourceManager.h"

namespace MainGame
{
	Tree::Tree()
	{
		VERTEX_3D vertex[4];

		vertex[0].Position = D3DXVECTOR3(-1.0f, 2.0f, 0.0f);
		vertex[0].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[0].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[0].TexCoord = D3DXVECTOR2(0.0f, 0.0f);

		vertex[1].Position = D3DXVECTOR3(1.0f, 2.0f, 0.0f);
		vertex[1].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[1].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[1].TexCoord = D3DXVECTOR2(1.0f, 0.0f);

		vertex[2].Position = D3DXVECTOR3(-1.0f, -1.0f, 0.0f);
		vertex[2].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[2].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[2].TexCoord = D3DXVECTOR2(0.0f, 1.0f);

		vertex[3].Position = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
		vertex[3].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		vertex[3].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
		vertex[3].TexCoord = D3DXVECTOR2(1.0f, 1.0f);

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

		//D3DX11CreateShaderResourceViewFromFile(Renderer::GetInstance().GetDevice(), "asset/texture/explosion.png", NULL, NULL, &m_texture, NULL);
		m_texture = ResourceManager::GetInstance().GetTextureData("asset/texture/Tree.png");
		assert(m_texture);

		Renderer::GetInstance().CreateVertexShader(&m_vertexShader, &m_vertexLayout, "unlitTextureVS.cso");

		Renderer::GetInstance().CreatePixelShader(&m_pixelShader, "unlitTexturePS.cso");

		auto transform = GetComponent<Transform>();
		transform->SetPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		transform->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		transform->SetScale(D3DXVECTOR3(2.0f, 2.0f, 2.0f));
	}

	Tree::~Tree()
	{
		m_vertexBuffer->Release();
		//m_texture->Release();

		m_vertexLayout->Release();
		m_vertexShader->Release();
		m_pixelShader->Release();
	}

	void Tree::Update()
	{
	}

	void Tree::Draw()
	{
		Renderer::GetInstance().GetDeviceContext()->Unmap(m_vertexBuffer, 0);

		//入力レイアウト設定
		Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_vertexLayout);

		//シェーダー設定
		Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
		Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

		//カメラのビューマトリクス取得
		Scene* scene = Manager::GetInstance().GetScene();
		Camera* camera = scene->GetGameObject<Camera>(scene->CAMERA);
		D3DXMATRIX view = camera->GetViewMatrix();

		//ビューの逆行列
		D3DXMATRIX invView;
		D3DXMatrixInverse(&invView, NULL, &view);
		invView._41 = 0.0f;
		invView._42 = 0.0f;
		invView._43 = 0.0f;

		//ワールドマトリクス設定
		D3DXMATRIX world = GetComponent<Transform>()->GetWorldMatrixInvView();
		Renderer::GetInstance().SetWorldMatrix(&world);

		//頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

		//マテリアル設定
		MATERIAL material;
		ZeroMemory(&material, sizeof(material));
		material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		Renderer::GetInstance().SetMaterial(material);

		//テクスチャ設定
		Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources(0, 1, &m_texture);

		//プリミティブトポロジ設定
		Renderer::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		//ポリゴン描画
		Renderer::GetInstance().GetDeviceContext()->Draw(4, 0);
	}
}