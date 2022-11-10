﻿#include "main.h"
#include "renderer.h"
#include "MeshField.h"
#include "ResourceManager.h"


#include <iostream>
using namespace std;



void MeshField::Init()
{
	if (!FileReader("Asset/terrain/heightmap01.bmp"))
	{
		return;
	}

	{
		for (int x = 0; x <= FIELD_X; x++)
		{
			for (int z = 0; z <= FIELD_X; z++)
			{
				//float y = g_fieldHeight[x][z];
				float y = m_heightMap[x][z];
				m_vertex[x][z].Position = D3DXVECTOR3((x - 10) * 5.0f, y, (z - 10) * -5.0f);
				m_vertex[x][z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				m_vertex[x][z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				m_vertex[x][z].TexCoord = D3DXVECTOR2(x * 0.5f, z * 0.5f);
			}
		}
		for (int x = 1; x <= FIELD_X - 1; x++)
		{
			for (int z = 1; z <= FIELD_Z - 1; z++)
			{
				D3DXVECTOR3 vx, vz, vn;
				vx = m_vertex[x + 1][z].Position - m_vertex[x - 1][z].Position;
				vz = m_vertex[x][z + 1].Position - m_vertex[x][z - 1].Position;

				D3DXVec3Cross(&vn, &vx, &vz); //外積
				D3DXVec3Normalize(&vn, &vn); 
				m_vertex[x][z].Normal = vn;
			}
		}

		//頂点バッファ生成	
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * (FIELD_X + 1) * (FIELD_Z + 1);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_vertex;

		Renderer::GetInstance().GetDevice()->CreateBuffer(&bd, &sd, &m_vertexBuffer);
	}

	{
		unsigned int index[INDEX_NUM];
		
		int i = 0;
		for (int x = 0; x < FIELD_X; x++)
		{
			for (int z = 0; z < (FIELD_Z + 1); z++)
			{
				index[i] = x * (FIELD_Z + 1) + z;
				i++;

				index[i] = (x + 1) * (FIELD_Z + 1) + z;
				i++;
			}

			if (x == (FIELD_X - 1))
				break;

			index[i] = (x + 1) * (FIELD_Z + 1) + FIELD_X;
			i++;

			index[i] = (x + 1) * (FIELD_Z + 1);
			i++;
		}
		
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * INDEX_NUM;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		Renderer::GetInstance().GetDevice()->CreateBuffer(&bd, &sd, &m_indexBuffer);
	}

	//テクスチャ読み込み
	m_texture = ResourceManager::GetInstance().GetTextureData("asset/texture/forest.jpg");

	assert(m_texture);

	Renderer::GetInstance().CreateVertexShader(&m_vertexShader, &m_vertexLayout, "vertexLightingVS.cso");
	Renderer::GetInstance().CreatePixelShader(&m_pixelShader, "vertexLightingPS.cso");

	m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

void MeshField::Uninit()
{
	m_vertexBuffer->Release();
	m_indexBuffer->Release();
	m_texture->Release();

	m_vertexLayout->Release();
	m_vertexShader->Release();
	m_pixelShader->Release();
}

void MeshField::Update()
{
}

void MeshField::Draw()
{
	//入力レイアウト設定
	Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_vertexLayout);

	//シェーダー設定
	Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);

	//ワールドマトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_rotation.y, m_rotation.x, m_rotation.z);
	D3DXMatrixTranslation(&trans, m_position.x, m_position.y, m_position.z);
	world = scale * rot * trans;
	Renderer::GetInstance().SetWorldMatrix(&world);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	//インデックスバッファ生成
	Renderer::GetInstance().GetDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

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
	Renderer::GetInstance().GetDeviceContext()->DrawIndexed(INDEX_NUM, 0,0);
}

float MeshField::GetHeight(D3DXVECTOR3 position)
{
	int x, z;

	x = static_cast<int>(position.x / 5.0f + 10.0f);
	z = static_cast<int>(position.z / -5.0f + 10.0f);

	D3DXVECTOR3 pos0, pos1, pos2, pos3;

	pos0 = m_vertex[x][z].Position;
	pos1 = m_vertex[x + 1][z].Position;
	pos2 = m_vertex[x][z + 1].Position;
	pos3 = m_vertex[x + 1][z + 1].Position;

	D3DXVECTOR3 v12, v1p, c;

	v12 = pos2 - pos1;
	v1p = position - pos1;

	D3DXVec3Cross(&c, &v12, &v1p);

	float py;
	D3DXVECTOR3 n;

	if (c.y > 0.0f)
	{
		D3DXVECTOR3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&n, &v10, &v12);
	}
	else
	{
		D3DXVECTOR3 v13;
		v13 = pos3 - pos1;
		D3DXVec3Cross(&n, &v12, &v13);
	}

	//高さ取得
	py = -((position.x - pos1.x) * n.x + (position.z - pos1.z) * n.z) / n.y + pos1.y;

	return py;
}


bool MeshField::FileReader(const char* filename)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, z, x, k;
	unsigned char* bitmapImage;
	unsigned char height;

	//ハイトマップをバイナリで開く
	error = fopen_s(&filePtr, filename, "rb");
	if (error != 0)
	{
		assert(error);
		return false;
	}

	//ファイルヘッダ読み込み
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	if (count != 1)
	{
		assert(count);
		return false;
	}

	//ビットマップヘッダ読み込み
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	if (count != 1)
	{
		assert(count);
		return false;
	}

	//地形の大きさ
	m_terrainWidth = bitmapInfoHeader.biWidth;
	m_terrainHeight = bitmapInfoHeader.biHeight;



	//画像サイズ計算
	imageSize = m_terrainWidth * (m_terrainHeight + 3) * 3;

	//メモリ割り当て
	bitmapImage = new unsigned char[imageSize];
	if (!bitmapImage)
	{
		assert(bitmapImage);

		return false;
	}

	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	//bmp読み込み
	count = fread(bitmapImage, sizeof(unsigned char), imageSize, filePtr);

	//ファイルを閉じる
	error = fclose(filePtr);
	if (error != 0)
	{
		assert(error);
		return false;
	}

	//ハイトマップデータ用の変数を作成
	m_heightMap.resize(m_terrainWidth);
	for (int i = 0; i < m_terrainWidth; i++)
	{
		m_heightMap[i].resize(m_terrainHeight);
	}

	//ハイトマップに代入
	k = 0;
	for (z = m_terrainHeight - 1; z >= 0; z--)
	{
		for (x = 0; x < m_terrainWidth; x++)
		{
			height = bitmapImage[k];

			float temp = (float)height;
			temp /= 5;

			m_heightMap[x][z] = temp;

			k += 3;
		}
	}

	delete[] bitmapImage;
	bitmapImage = 0;

	return true;
}
