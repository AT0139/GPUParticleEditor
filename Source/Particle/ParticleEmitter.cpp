#include "ParticleEmitter.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "MainCamera.h"
#include "SceneManager.h"


static const float MIN_RAND = -1.0f;
static const float MAX_RAND = 2.0f;


ParticleEmitter::ParticleEmitter(EmitterInitData initData)
	: m_particleBuffer(nullptr)
	, m_positionBuffer(nullptr)
	, m_resultBuffer(nullptr)
	, m_particleNum(initData.maxNum)
	, m_initData(initData)
	, m_createCount(initData.createInterval)

{
	VERTEX_3D vertex[4];

	vertex[0].position = Vector3(-initData.size.x, initData.size.y, 0.0f);
	vertex[0].normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[0].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].texCoord = Vector2(0.0f, 0.0f);

	vertex[1].position = Vector3(initData.size.x, initData.size.y, 0.0f);
	vertex[1].normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[1].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].texCoord = Vector2(1.0f, 0.0f);

	vertex[2].position = Vector3(-initData.size.x, -initData.size.y, 0.0f);
	vertex[2].normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[2].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].texCoord = Vector2(0.0f, 1.0f);

	vertex[3].position = Vector3(initData.size.x, -initData.size.y, 0.0f);
	vertex[3].normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[3].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].texCoord = Vector2(1.0f, 1.0f);

	m_texture = ResourceManager::GetInstance().GetTextureData(initData.filePath);

	m_particle.reset(new ParticleCompute[m_particleNum]);

	for (int i = 0; i < m_particleNum; i++)
	{
		m_particle[i].vel = Vector3::Zero;
		m_particle[i].life = 0;
		m_particle[i].pos = Vector3(0.0f, 0.0f, 0.0f);
	}

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

	Renderer::GetInstance().CreateStructuredBuffer(sizeof(ParticleCompute), m_particleNum, nullptr, &m_particleBuffer, true);
	Renderer::GetInstance().CreateStructuredBuffer(sizeof(Vector3), m_particleNum, nullptr, &m_positionBuffer, true);
	Renderer::GetInstance().CreateStructuredBuffer(sizeof(int), m_particleNum, nullptr, &m_lifeBuffer, true);
	Renderer::GetInstance().CreateStructuredBuffer(sizeof(ParticleCompute), m_particleNum, nullptr, &m_resultBuffer);


	Renderer::GetInstance().CreateBufferSRV(m_particleBuffer, &m_particleSRV);
	Renderer::GetInstance().CreateBufferSRV(m_positionBuffer, &m_positionSRV);
	Renderer::GetInstance().CreateBufferSRV(m_lifeBuffer, &m_lifeSRV);


	Renderer::GetInstance().CreateBufferUAV(m_resultBuffer, &m_resultUAV);

	Renderer::GetInstance().CreateComputeShader(&m_computeShader, "Shader/ParticleCS.cso");
}

ParticleEmitter::~ParticleEmitter()
{
	m_computeShader->Release();

	m_particleBuffer->Release();
	m_resultBuffer->Release();
	m_positionBuffer->Release();
	m_lifeBuffer->Release();

	m_particleSRV->Release();
	m_positionSRV->Release();
	m_lifeSRV->Release();

	m_resultUAV->Release();

	m_vertexBuffer->Release();
}

void ParticleEmitter::Update()
{
	m_createCount++;
	if (m_createCount >= m_initData.createInterval)
	{
		CreateParticle();
		m_createCount = 0;
	}

	auto context = Renderer::GetInstance().GetDeviceContext();

	D3D11_MAPPED_SUBRESOURCE subRes;
	context->Map(m_particleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes);
	ParticleCompute* pBufType = (ParticleCompute*)subRes.pData;
	memcpy(subRes.pData, m_particle.get(), sizeof(ParticleCompute) * m_particleNum);
	context->Unmap(m_particleBuffer, 0);

	//　コンピュートシェーダー実行
	ID3D11ShaderResourceView* pSRVs[1] = { m_particleSRV };
	context->CSSetShaderResources(0, 1, pSRVs);
	context->CSSetShader(m_computeShader, nullptr, 0);
	context->CSSetUnorderedAccessViews(0, 1, &m_resultUAV, 0);
	context->Dispatch(256, 1, 1);

	// 戻った計算結果をバッファに入れる
	ID3D11Buffer* pBufDbg = Renderer::GetInstance().CreateAndCopyToBuffer(m_resultBuffer);
	D3D11_MAPPED_SUBRESOURCE subRes2;
	context->Map(pBufDbg, 0, D3D11_MAP_READ, 0, &subRes2);
	ParticleCompute* pBufType2 = reinterpret_cast<ParticleCompute*>(subRes2.pData);
	memcpy(m_particle.get(), pBufType2, sizeof(ParticleCompute) * m_particleNum);
	context->Unmap(pBufDbg, 0);
	pBufDbg->Release();

	// 座標を座標バッファに入れる(頂点シェーダーで使う)
	{
		context->Map(m_positionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes2);
		Vector3* BufType = (Vector3*)subRes2.pData;
		for (int v = 0; v < m_particleNum; v++)
		{
			BufType[v] = m_particle[v].pos;
		}
		context->Unmap(m_positionBuffer, 0);
	}
	{
		context->Map(m_lifeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes2);
		int* BufType = (int*)subRes2.pData;
		for (int v = 0; v < m_particleNum; v++)
		{
			BufType[v] = m_particle[v].life;
		}
		context->Unmap(m_lifeBuffer, 0);
	}
}

void ParticleEmitter::Draw()
{
	auto context = Renderer::GetInstance().GetDeviceContext();

	// ビルボード
	auto scene = SceneManager::GetInstance().GetScene();
	Matrix view = scene->GetCamera()->GetViewMatrix();
	Matrix invView = view.Invert();
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;

	// ワールド座標、スケールなどの処理
	Matrix world, scale, trans;
	scale = Matrix::CreateScale(Vector3(1,1,1));
	trans = Matrix::CreateTranslation(m_managerPosition + m_offsetPosition);
	world = scale * invView * trans;
	Renderer::GetInstance().SetWorldMatrix(&world);

	ShaderManager::GetInstance().Set(SHADER_TYPE::PARTICLE);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	context->PSSetShaderResources(0, 1, &m_texture); // テクスチャ設定（あれば）
	context->VSSetShaderResources(2, 1, &m_positionSRV); // VSに入れる座標設定
	context->VSSetShaderResources(3, 1, &m_lifeSRV);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	context->DrawInstanced(4, m_particleNum, 0, 0);
}

void ParticleEmitter::CreateParticle()
{
	int count = 0;
	for (int i = 0; i < m_particleNum; i++)
	{
		if (m_particle[i].life <= 0)
		{
			float x = Utility::FloatRand(MIN_RAND, MAX_RAND);
			float y = Utility::FloatRand(MIN_RAND, MAX_RAND);
			float z = Utility::FloatRand(MIN_RAND, MAX_RAND);
			m_particle[i].vel = (Vector3(x, y, z) + m_initData.direction) * 0.5f;
			m_particle[i].life = m_initData.life;
			m_particle[i].pos = Vector3(0.0f, 0.0f, 0.0f);
			count++;
			if (m_initData.createOnceNum <= count)
				return;
		}
	}
}
