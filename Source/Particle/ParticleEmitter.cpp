#include "ParticleEmitter.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "Camera.h"
#include "Manager.h"

static const UINT PARTICLE_AMOUNT = 100;

ParticleEmitter::ParticleEmitter(EmitterInitData initData)
	: m_particleBuffer(nullptr)
	, m_positionBuffer(nullptr)
	, m_resultBuffer(nullptr)
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

	m_particle.reset(new ParticleCompute[PARTICLE_AMOUNT]);

	for (int i = 0; i < PARTICLE_AMOUNT; i++)
	{
		float x = Utility::FloatRand(-0.4f, 1.4f);
		float y = Utility::FloatRand(-0.4f, 1.4f);
		float z = Utility::FloatRand(-0.4f, 1.4f);
		m_particle[i].vel = Vector3(x, y, z) + initData.direction;
		m_particle[i].life = initData.life;
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
	Renderer::GetInstance().CreateVertexShader(&m_vertexShader, &m_vertexLayout, "ParticleTextureVS.cso");
	Renderer::GetInstance().CreatePixelShader(&m_pixelShader, "ParticleTexturePS.cso");

	Renderer::GetInstance().CreateStructuredBuffer(sizeof(ParticleCompute), PARTICLE_AMOUNT, nullptr, &m_particleBuffer, true);
	Renderer::GetInstance().CreateStructuredBuffer(sizeof(Vector3), PARTICLE_AMOUNT, nullptr, &m_positionBuffer, true);
	Renderer::GetInstance().CreateStructuredBuffer(sizeof(ParticleCompute), PARTICLE_AMOUNT, nullptr, &m_resultBuffer);

	Renderer::GetInstance().CreateBufferSRV(m_particleBuffer, &m_particleSRV);
	Renderer::GetInstance().CreateBufferSRV(m_positionBuffer, &m_positionSRV);

	Renderer::GetInstance().CreateBufferUAV(m_resultBuffer, &m_resultUAV);

	Renderer::GetInstance().CreateComputeShader(&m_computeShader, "ComputeShader.cso");
}

ParticleEmitter::~ParticleEmitter()
{
	m_computeShader->Release();

	m_particleBuffer->Release();
	m_resultBuffer->Release();
	m_positionBuffer->Release();


	m_particleSRV->Release();
	m_positionSRV->Release();

	m_resultUAV->Release();

	m_vertexBuffer->Release();
	m_vertexShader->Release();
	m_pixelShader->Release();
	m_vertexLayout->Release();
}

void ParticleEmitter::Update()
{
	D3D11_MAPPED_SUBRESOURCE subRes;  
	Renderer::GetInstance().GetDeviceContext()->Map(m_particleBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes);
	ParticleCompute* pBufType = (ParticleCompute*)subRes.pData;
	memcpy(subRes.pData, m_particle.get() , sizeof(ParticleCompute) * PARTICLE_AMOUNT);
	Renderer::GetInstance().GetDeviceContext()->Unmap(m_particleBuffer, 0);

	//　コンピュートシェーダー実行
	ID3D11ShaderResourceView* pSRVs[1] = { m_particleSRV };
	Renderer::GetInstance().GetDeviceContext()->CSSetShaderResources(0, 1, pSRVs);
	Renderer::GetInstance().GetDeviceContext()->CSSetShader(m_computeShader, nullptr, 0);
	Renderer::GetInstance().GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &m_resultUAV, 0);
	Renderer::GetInstance().GetDeviceContext()->Dispatch(256, 1, 1);

	// 戻った計算結果をバッファに入れる
	ID3D11Buffer* pBufDbg = Renderer::GetInstance().CreateAndCopyToBuffer(m_resultBuffer);
	D3D11_MAPPED_SUBRESOURCE subRes2;
	Renderer::GetInstance().GetDeviceContext()->Map(pBufDbg, 0, D3D11_MAP_READ, 0, &subRes2);
	ParticleCompute* pBufType2 = reinterpret_cast<ParticleCompute*>(subRes2.pData);
	memcpy(m_particle.get(), pBufType2, sizeof(ParticleCompute) * PARTICLE_AMOUNT);
	Renderer::GetInstance().GetDeviceContext()->Unmap(pBufDbg, 0);
	pBufDbg->Release();

	// 座標を座標バッファに入れる(頂点シェーダーで使う)
	Renderer::GetInstance().GetDeviceContext()->Map(m_positionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes2);
	Vector3* BufType = (Vector3*)subRes2.pData;
	for (int v = 0; v < PARTICLE_AMOUNT; v++)
	{
		BufType[v] = m_particle[v].pos;
	}
	Renderer::GetInstance().GetDeviceContext()->Unmap(m_positionBuffer, 0);
}

void ParticleEmitter::Draw()
{
	auto context = Renderer::GetInstance().GetDeviceContext();
	//入力レイアウト設定
	context->IASetInputLayout(m_vertexLayout);
	// ビルボード
	auto scene = Manager::GetInstance().GetScene();
	Matrix view = scene->GetGameObject<MainGame::Camera>(scene->CAMERA)->GetViewMatrix();
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

	//シェーダー設定
	context->VSSetShader(m_vertexShader, NULL, 0);
	context->PSSetShader(m_pixelShader, NULL, 0);

	//頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	context->PSSetShaderResources(0, 1, &m_texture); // テクスチャ設定（あれば）
	context->VSSetShaderResources(2, 1, &m_positionSRV); // VSに入れる座標設定
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	context->DrawInstanced(4, PARTICLE_AMOUNT, 0, 0);
}

void ParticleEmitter::Load(const wchar_t* filePath)
{
	m_texture = ResourceManager::GetInstance().GetTextureData(filePath);
}