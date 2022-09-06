#include "main.h"
#include "renderer.h"
#include "AnimationModel.h"


void AnimationModel::Load(const char* FileName)
{
	m_aiScene = aiImportFile(FileName, aiProcessPreset_TargetRealtime_MaxQuality);
	assert(m_aiScene);

	m_vertexBuffer = new ID3D11Buffer * [m_aiScene->mNumMeshes];
	m_indexBuffer = new ID3D11Buffer * [m_aiScene->mNumMeshes];

	for (unsigned int m = 0; m < m_aiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_aiScene->mMeshes[m];

		//頂点バッファ生成
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = D3DXVECTOR3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex[v].Normal = D3DXVECTOR3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].TexCoord = D3DXVECTOR2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);
				vertex[v].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;

			Renderer::GetInstance()->GetDevice()->CreateBuffer(&bd, &sd, &m_vertexBuffer[m]);

			delete[] vertex;
		}

		//インデックスバッファ生成
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			for (unsigned int f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace* face = &mesh->mFaces[f];

				assert(face->mNumIndices == 3);

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			Renderer::GetInstance()->GetDevice()->CreateBuffer(&bd, &sd, &m_indexBuffer[m]);

			delete[] index;
		}
	}
}

void AnimationModel::Unload()
{
	for (unsigned int m = 0; m < m_aiScene->mNumMeshes; m++)
	{
		m_vertexBuffer[m]->Release();
		m_indexBuffer[m]->Release();
	}
	delete[] m_vertexBuffer;
	delete[] m_indexBuffer;

	aiReleaseImport(m_aiScene);
}

void AnimationModel::Update()
{
}

void AnimationModel::Draw()
{
	//プリミティブトポロジ設定
	Renderer::GetInstance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//マテリアル設定
	MATERIAL material;
	ZeroMemory(&material,sizeof(material));
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	Renderer::GetInstance()->SetMaterial(material);

	for (unsigned int m = 0; m < m_aiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_aiScene->mMeshes[m];

		//頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		Renderer::GetInstance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer[m], &stride, &offset);

		//インデックスバッファ設定
		Renderer::GetInstance()->GetDeviceContext()->IASetIndexBuffer(m_indexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		//ポリゴン描画
		Renderer::GetInstance()->GetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);

	}
}
