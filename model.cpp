
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "main.h"
#include "renderer.h"
#include "model.h"

Model::Model()
	: m_vertexBuffer(nullptr)
	, m_indexBuffer(nullptr)
	, m_pSubsetArray(nullptr)
	, m_subsetNum(0)
{}

Model::Model(const char* fileName)
{
	Load(fileName);
}

void Model::Draw()
{

	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetInstance().GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

	// インデックスバッファ設定
	Renderer::GetInstance().GetDeviceContext()->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// プリミティブトポロジ設定
	Renderer::GetInstance().GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	for( unsigned int i = 0; i < m_subsetNum; i++ )
	{
		// マテリアル設定
		Renderer::GetInstance().SetMaterial( m_pSubsetArray[i].material.material );

		// テクスチャ設定
		Renderer::GetInstance().GetDeviceContext()->PSSetShaderResources( 0, 1, &m_pSubsetArray[i].material.pTexture );

		// ポリゴン描画
		Renderer::GetInstance().GetDeviceContext()->DrawIndexed( m_pSubsetArray[i].indexNum, m_pSubsetArray[i].startIndex, 0 );
	}

}



void Model::Load( const char *FileName )
{

	MODEL model;
	LoadObj( FileName, &model );



	// 頂点バッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof(bd) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( VERTEX_3D ) * model.vertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory( &sd, sizeof(sd) );
		sd.pSysMem = model.vertexArray;

		Renderer::GetInstance().GetDevice()->CreateBuffer( &bd, &sd, &m_vertexBuffer );
	}


	// インデックスバッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof(bd) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof( unsigned int ) * model.indexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory( &sd, sizeof(sd) );
		sd.pSysMem = model.indexArray;

		Renderer::GetInstance().GetDevice()->CreateBuffer( &bd, &sd, &m_indexBuffer );
	}

	// サブセット設定
	{
		m_pSubsetArray = new SUBSET[ model.subsetNum ];
		m_subsetNum = model.subsetNum;

		for( unsigned int i = 0; i < model.subsetNum; i++ )
		{
			m_pSubsetArray[i].startIndex = model.subsetArray[i].startIndex;
			m_pSubsetArray[i].indexNum = model.subsetArray[i].indexNum;

			m_pSubsetArray[i].material.material = model.subsetArray[i].material.material;

			m_pSubsetArray[i].material.pTexture = NULL;

			D3DX11CreateShaderResourceViewFromFile(Renderer::GetInstance().GetDevice(),
				model.subsetArray[i].material.textureName,
				NULL,
				NULL,
				&m_pSubsetArray[i].material.pTexture,
				NULL);

			assert(m_pSubsetArray[i].material.pTexture);

		}
	}

	delete[] model.vertexArray;
	delete[] model.indexArray;
	delete[] model.subsetArray;

}





void Model::Unload()
{
	m_vertexBuffer->Release();
	m_indexBuffer->Release();

	for (unsigned int i = 0; i < m_subsetNum; i++)
	{
		m_pSubsetArray[i].material.pTexture->Release();
	}

	delete[] m_pSubsetArray;

}





//モデル読込////////////////////////////////////////////
void Model::LoadObj( const char *FileName, MODEL *Model )
{

	char dir[MAX_PATH];
	strcpy (dir, FileName );
	PathRemoveFileSpec(dir);





	D3DXVECTOR3	*positionArray;
	D3DXVECTOR3	*normalArray;
	D3DXVECTOR2	*texcoordArray;

	unsigned int	positionNum = 0;
	unsigned int	normalNum = 0;
	unsigned int	texcoordNum = 0;
	unsigned int	vertexNum = 0;
	unsigned int	indexNum = 0;
	unsigned int	in = 0;
	unsigned int	subsetNum = 0;

	MODEL_MATERIAL	*materialArray = NULL;
	unsigned int	materialNum = 0;

	char str[256];
	char *s;
	char c;


	FILE *file;
	file = fopen( FileName, "rt" );
	assert(file);



	//要素数カウント
	while( true )
	{
		(void)fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;

		if( strcmp( str, "v" ) == 0 )
		{
			positionNum++;
		}
		else if( strcmp( str, "vn" ) == 0 )
		{
			normalNum++;
		}
		else if( strcmp( str, "vt" ) == 0 )
		{
			texcoordNum++;
		}
		else if( strcmp( str, "usemtl" ) == 0 )
		{
			subsetNum++;
		}
		else if( strcmp( str, "f" ) == 0 )
		{
			in = 0;

			do
			{
				(void)fscanf( file, "%s", str );
				vertexNum++;
				in++;
				c = fgetc( file );
			}
			while( c != '\n' && c!= '\r' );

			//四角は三角に分割
			if( in == 4 )
				in = 6;

			indexNum += in;
		}
	}


	//メモリ確保
	positionArray = new D3DXVECTOR3[ positionNum ];
	normalArray = new D3DXVECTOR3[ normalNum ];
	texcoordArray = new D3DXVECTOR2[ texcoordNum ];


	Model->vertexArray = new VERTEX_3D[ vertexNum ];
	Model->vertexNum = vertexNum;

	Model->indexArray = new unsigned int[ indexNum ];
	Model->indexNum = indexNum;

	Model->subsetArray = new SUBSET[ subsetNum ];
	Model->subsetNum = subsetNum;




	//要素読込
	D3DXVECTOR3 *position = positionArray;
	D3DXVECTOR3 *normal = normalArray;
	D3DXVECTOR2 *texcoord = texcoordArray;

	unsigned int vc = 0;
	unsigned int ic = 0;
	unsigned int sc = 0;


	fseek( file, 0, SEEK_SET );

	while( true )
	{
		(void)fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;

		if( strcmp( str, "mtllib" ) == 0 )
		{
			//マテリアルファイル
			(void)fscanf( file, "%s", str );

			char path[256];
			strcpy( path, dir );
			strcat( path, "\\" );
			strcat( path, str );

			LoadMaterial( path, &materialArray, &materialNum );
		}
		else if( strcmp( str, "o" ) == 0 )
		{
			//オブジェクト名
			(void)fscanf( file, "%s", str );
		}
		else if( strcmp( str, "v" ) == 0 )
		{
			//頂点座標
			(void)fscanf( file, "%f", &position->x );
			(void)fscanf( file, "%f", &position->y );
			(void)fscanf( file, "%f", &position->z );
			position++;
		}
		else if( strcmp( str, "vn" ) == 0 )
		{
			//法線
			(void)fscanf( file, "%f", &normal->x );
			(void)fscanf( file, "%f", &normal->y );
			(void)fscanf( file, "%f", &normal->z );
			normal++;
		}
		else if( strcmp( str, "vt" ) == 0 )
		{
			//テクスチャ座標
			(void)fscanf( file, "%f", &texcoord->x );
			(void)fscanf( file, "%f", &texcoord->y );
			texcoord->y = 1.0f - texcoord->y;
			texcoord++;
		}
		else if( strcmp( str, "usemtl" ) == 0 )
		{
			//マテリアル
			(void)fscanf( file, "%s", str );

			if( sc != 0 )
				Model->subsetArray[ sc - 1 ].indexNum = ic - Model->subsetArray[ sc - 1 ].startIndex;

			Model->subsetArray[ sc ].startIndex = ic;


			for( unsigned int i = 0; i < materialNum; i++ )
			{
				if( strcmp( str, materialArray[i].name ) == 0 )
				{
					Model->subsetArray[ sc ].material.material = materialArray[i].material;
					strcpy( Model->subsetArray[ sc ].material.textureName, materialArray[i].textureName );
					strcpy( Model->subsetArray[ sc ].material.name, materialArray[i].name );

					break;
				}
			}

			sc++;
			
		}
		else if( strcmp( str, "f" ) == 0 )
		{
			//面
			in = 0;

			do
			{
				(void)fscanf( file, "%s", str );

				s = strtok( str, "/" );	
				Model->vertexArray[vc].Position = positionArray[ atoi( s ) - 1 ];
				if( s[ strlen( s ) + 1 ] != '/' )
				{
					//テクスチャ座標が存在しない場合もある
					s = strtok( NULL, "/" );
					Model->vertexArray[vc].TexCoord = texcoordArray[ atoi( s ) - 1 ];
				}
				s = strtok( NULL, "/" );	
				Model->vertexArray[vc].Normal = normalArray[ atoi( s ) - 1 ];

				Model->vertexArray[vc].Diffuse = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );

				Model->indexArray[ic] = vc;
				ic++;
				vc++;

				in++;
				c = fgetc( file );
			}
			while( c != '\n' && c != '\r' );

			//四角は三角に分割
			if( in == 4 )
			{
				Model->indexArray[ic] = vc - 4;
				ic++;
				Model->indexArray[ic] = vc - 2;
				ic++;
			}
		}
	}


	if( sc != 0 )
		Model->subsetArray[ sc - 1 ].indexNum = ic - Model->subsetArray[ sc - 1 ].startIndex;


	fclose( file );


	delete[] positionArray;
	delete[] normalArray;
	delete[] texcoordArray;
	delete[] materialArray;
}




//マテリアル読み込み///////////////////////////////////////////////////////////////////
void Model::LoadMaterial( const char *FileName, MODEL_MATERIAL **MaterialArray, unsigned int *MaterialNum )
{

	char dir[MAX_PATH];
	strcpy(dir, FileName);
	PathRemoveFileSpec(dir);



	char str[256];

	FILE *file;
	file = fopen( FileName, "rt" );
	assert(file);

	MODEL_MATERIAL *materialArray;
	unsigned int materialNum = 0;

	//要素数カウント
	while( true )
	{
		(void)fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;


		if( strcmp( str, "newmtl" ) == 0 )
		{
			materialNum++;
		}
	}


	//メモリ確保
	materialArray = new MODEL_MATERIAL[ materialNum ];


	//要素読込
	int mc = -1;

	fseek( file, 0, SEEK_SET );

	while( true )
	{
		(void)fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;


		if( strcmp( str, "newmtl" ) == 0 )
		{
			//マテリアル名
			mc++;
			(void)fscanf( file, "%s", materialArray[ mc ].name );
			strcpy( materialArray[ mc ].textureName, "" );

			materialArray[mc].material.Emission.r = 0.0f;
			materialArray[mc].material.Emission.g = 0.0f;
			materialArray[mc].material.Emission.b = 0.0f;
			materialArray[mc].material.Emission.a = 0.0f;
		}
		else if( strcmp( str, "Ka" ) == 0 )
		{
			//アンビエント
			(void)fscanf( file, "%f", &materialArray[ mc ].material.Ambient.r );
			(void)fscanf( file, "%f", &materialArray[ mc ].material.Ambient.g );
			(void)fscanf( file, "%f", &materialArray[ mc ].material.Ambient.b );
			materialArray[ mc ].material.Ambient.a = 1.0f;
		}
		else if( strcmp( str, "Kd" ) == 0 )
		{
			//ディフューズ
			(void)fscanf( file, "%f", &materialArray[ mc ].material.Diffuse.r );
			(void)fscanf( file, "%f", &materialArray[ mc ].material.Diffuse.g );
			(void)fscanf( file, "%f", &materialArray[ mc ].material.Diffuse.b );
			materialArray[ mc ].material.Diffuse.a = 1.0f;
		}
		else if( strcmp( str, "Ks" ) == 0 )
		{
			//スペキュラ
			(void)fscanf( file, "%f", &materialArray[ mc ].material.Specular.r );
			(void)fscanf( file, "%f", &materialArray[ mc ].material.Specular.g );
			(void)fscanf( file, "%f", &materialArray[ mc ].material.Specular.b );
			materialArray[ mc ].material.Specular.a = 1.0f;
		}
		else if( strcmp( str, "Ns" ) == 0 )
		{
			//スペキュラ強度
			(void)fscanf( file, "%f", &materialArray[ mc ].material.Shininess );
		}
		else if( strcmp( str, "d" ) == 0 )
		{
			//アルファ
			(void)fscanf( file, "%f", &materialArray[ mc ].material.Diffuse.a );
		}
		else if( strcmp( str, "map_Kd" ) == 0 )
		{
			//テクスチャ
			(void)fscanf( file, "%s", str );

			char path[256];
			strcpy( path, dir );
			strcat( path, "\\" );
			strcat( path, str );

			strcat( materialArray[ mc ].textureName, path );
		}
	}

	fclose(file);

	*MaterialArray = materialArray;
	*MaterialNum = materialNum;
}

