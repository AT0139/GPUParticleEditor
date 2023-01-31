#include "ParticleEmitter.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "MainCamera.h"
#include "SceneManager.h"
#include <omp.h>

static const float MIN_RAND = -1.0f;
static const float MAX_RAND = 2.0f;

//todo :　バッファの最適化

ParticleEmitter::ParticleEmitter(EmitterInitData initData)
	: m_particleComputeBuffer(nullptr)
	, m_parameterBuffer(nullptr)
	, m_resultBuffer(nullptr)
	, m_particleNum(initData.maxNum)
	, m_initData(initData)
	, m_gravity(false)
	, m_spawnType(initData.spawnType)
	, m_createInterval(1)
	, m_createCount(0)
	, m_createOnceNum(0)
{
	VERTEX_3D vertex[1];

	vertex[0].position = Vector3(0.0f, 0.0f, 0.0f);
	vertex[0].normal = Vector3(0.0f, 1.0f, 0.0f);
	vertex[0].diffuse = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].texCoord = Vector2(0.0f, 0.0f);

	//頂点バッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.pSysMem = vertex;

	Renderer::GetInstance().GetDevice()->CreateBuffer(&bd, &sd, &m_vertexBuffer);

	m_texture = ResourceManager::GetInstance().GetTextureData(initData.filePath);

	m_particle.reset(new ParticleCompute[m_particleNum]);

	for (int i = 0; i < m_particleNum; i++)
	{
		m_particle[i].velocity = Vector3::Zero;
		m_particle[i].speed = Vector3::Zero;
		m_particle[i].life = 0;
		m_particle[i].pos = Vector3(0.0f, 0.0f, 0.0f);
	}

	
	BufferInfo info = {};
	info.gravity = initData.gravity;
	
	Renderer::GetInstance().CreateConstantBuffer(&m_gravityBuffer, nullptr, sizeof(BufferInfo), sizeof(float), 7);
	Renderer::GetInstance().GetDeviceContext()->UpdateSubresource(m_gravityBuffer, 0, NULL, &m_bufferInfo, 0, 0);

	Renderer::GetInstance().CreateStructuredBuffer(sizeof(ParticleCompute), m_particleNum, nullptr, &m_particleComputeBuffer, true);
	Renderer::GetInstance().CreateStructuredBuffer(sizeof(ParticleParameter), m_particleNum, nullptr, &m_parameterBuffer, true);
	Renderer::GetInstance().CreateStructuredBuffer(sizeof(ParticleCompute), m_particleNum, nullptr, &m_resultBuffer);


	Renderer::GetInstance().CreateBufferSRV(m_particleComputeBuffer, &m_particleSRV);
	Renderer::GetInstance().CreateBufferSRV(m_parameterBuffer, &m_parameterSRV);


	Renderer::GetInstance().CreateBufferUAV(m_resultBuffer, &m_resultUAV);

	Renderer::GetInstance().CreateComputeShader(&m_computeShader, "Shader/ParticleCS.cso");
	Renderer::GetInstance().CreateGeometryShader(&m_geometryShader, "Shader/GeometryShader.cso");
}

ParticleEmitter::~ParticleEmitter()
{
	m_computeShader->Release();
	m_geometryShader->Release();

	m_particleComputeBuffer->Release();
	m_resultBuffer->Release();
	m_parameterBuffer->Release();

	m_particleSRV->Release();
	m_parameterSRV->Release();

	m_resultUAV->Release();

	m_vertexBuffer->Release();
}

void ParticleEmitter::Update()
{
	//パーティクル生成
	m_createCount += 1;
	while (m_createCount >= m_createInterval)
	{
		CreateParticle(m_createOnceNum);
		m_createCount -= m_createInterval;
	}
	if (m_initData.life != m_bufferInfo.maxLife)
	{
		SetLife(m_initData.life);
	}


	auto context = Renderer::GetInstance().GetDeviceContext();

	D3D11_MAPPED_SUBRESOURCE subRes;
	context->Map(m_particleComputeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes);
	ParticleCompute* pBufType = (ParticleCompute*)subRes.pData;
	memcpy(subRes.pData, m_particle.get(), sizeof(ParticleCompute) * m_particleNum);
	context->Unmap(m_particleComputeBuffer, 0);

	//　コンピュートシェーダー実行
	ID3D11Buffer* pCBs[1] = { m_gravityBuffer };
	context->CSSetConstantBuffers(7, 1, pCBs);
	ID3D11ShaderResourceView* pSRVs[1] = { m_particleSRV };
	context->CSSetShaderResources(0, 1, pSRVs);
	auto depthTexture = Renderer::GetInstance().GetShadowDepthTexture();
	context->CSSetShaderResources(1, 1, &depthTexture);

	context->CSSetShader(m_computeShader, nullptr, 0);
	context->CSSetUnorderedAccessViews(0, 1, &m_resultUAV, 0);
	context->Dispatch(512, 1, 1);

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
		context->Map(m_parameterBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes2);
		ParticleParameter* BufType = (ParticleParameter*)subRes2.pData;
#pragma omp parallel for
		for (int v = 0; v < m_particleNum; v++)
		{
			BufType[v].pos = m_particle[v].pos;
			BufType[v].size = m_particle[v].size;
			BufType[v].color = m_particle[v].color;
		}
		context->Unmap(m_parameterBuffer, 0);
	}
}

void ParticleEmitter::Draw()
{
	auto context = Renderer::GetInstance().GetDeviceContext();

	// ビルボード
	auto scene = SceneManager::GetInstance().GetScene();
	Matrix view = scene->GetCamera()->GetViewMatrix();

	// ワールド座標、スケールなどの処理
	Matrix world, scale, trans;
	scale = Matrix::CreateScale(Vector3(1,1,1));
	trans = Matrix::CreateTranslation(m_managerPosition + m_offsetPosition);
	world = scale * view * trans;

	Renderer::GetInstance().SetWorldMatrix(&world);

	ShaderManager::GetInstance().Set(SHADER_TYPE::PARTICLE);
	context->GSSetShader(m_geometryShader, NULL, 0);

	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.diffuse = m_initData.color;
	Renderer::GetInstance().SetMaterial(material);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	context->PSSetShaderResources(0, 1, &m_texture); // テクスチャ設定（あれば）
	context->VSSetShaderResources(2, 1, &m_parameterSRV); // VSに入れる座標設定

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	context->DrawInstanced(1, m_particleNum, 0, 0);

	context->GSSetShader(NULL, NULL, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

}

void ParticleEmitter::SetInitialSize(Vector2 size)
{
	m_bufferInfo.initialSize = size;
	Renderer::GetInstance().GetDeviceContext()->UpdateSubresource(m_gravityBuffer, 0, NULL, &m_bufferInfo, 0, 0);
}

void ParticleEmitter::SetFinalSize(Vector2 size)
{
	m_bufferInfo.finalSize = size;
	Renderer::GetInstance().GetDeviceContext()->UpdateSubresource(m_gravityBuffer, 0, NULL, &m_bufferInfo, 0, 0);
}

void ParticleEmitter::SetInitialColor(Vector4 color)
{
	m_bufferInfo.initialColor = color;
	Renderer::GetInstance().GetDeviceContext()->UpdateSubresource(m_gravityBuffer, 0, NULL, &m_bufferInfo, 0, 0);
}

void ParticleEmitter::SetFinalColor(Vector4 color)
{
	m_bufferInfo.finalColor = color;
	Renderer::GetInstance().GetDeviceContext()->UpdateSubresource(m_gravityBuffer, 0, NULL, &m_bufferInfo, 0, 0);
}

void ParticleEmitter::SetGravity(Vector3 power)
{
	m_bufferInfo.gravity = power;
	Renderer::GetInstance().GetDeviceContext()->UpdateSubresource(m_gravityBuffer, 0, NULL, &m_bufferInfo, 0, 0);
}

void ParticleEmitter::SetLife(int life)
{
	m_bufferInfo.maxLife = life;
	Renderer::GetInstance().GetDeviceContext()->UpdateSubresource(m_gravityBuffer, 0, NULL, &m_bufferInfo, 0, 0);
}

void ParticleEmitter::CreateParticle(int createNum)
{
	int count = 0;

	for (int i = 0; i < m_particleNum; i++)
	{
		if (m_particle[i].life <= 0)
		{
			Vector3 vel = {};
			switch (m_velocityType)
			{
			case ADD_VELOCITY_TYPE::NONE:
	
				break;
			case ADD_VELOCITY_TYPE::IN_CONE:
				vel.x = Utility::FloatRand(MIN_RAND, MAX_RAND);
				vel.y = Utility::FloatRand(MIN_RAND, MAX_RAND);
				vel.z = Utility::FloatRand(MIN_RAND, MAX_RAND);
				break;
			default:
				break;
			}
		
			m_particle[i].velocity = (vel) * 0.5f;
			m_particle[i].speed = Vector3::Zero;
			m_particle[i].life = m_initData.life;
			m_particle[i].pos = Vector3(0.0f, 0.0f, 0.0f);
			count++;
			if (createNum <= count)
				return;
		}
	}
}

void ParticleEmitter::SetVelocity(Vector3 vel, ADD_VELOCITY_TYPE type)
{
	m_velocityType = type;
	m_bufferInfo.velocity = vel;
	Renderer::GetInstance().GetDeviceContext()->UpdateSubresource(m_gravityBuffer, 0, NULL, &m_bufferInfo, 0, 0);
}

void ParticleEmitter::SetSpawnRate(float rate)
{
	m_spawnRate = rate;
	float createFrame = FPS / rate;
	if (createFrame < 1)
	{
		m_createOnceNum = 1;
	}
	else
	{
		m_createOnceNum = 1;
	}
	m_createInterval = createFrame;
}
