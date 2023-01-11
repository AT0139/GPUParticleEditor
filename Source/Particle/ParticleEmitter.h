#pragma once


struct ParticleCompute
{
	Vector3 pos;
	Vector3 vel;
	int life;
};

struct EmitterInitData
{
	Vector2 size			= Vector2(1.0f, 1.0f);
	Vector3 direction		= Vector3::Zero;
	int life				= 10;
	const wchar_t* filePath	= L"Asset\\Texture\\BlueBloom.png";
	int maxNum				= 10000;
	int createOnceNum		= 100;
	int createInterval		= 10;
};

class ParticleEmitter
{
public:
	ParticleEmitter(EmitterInitData initData);
	~ParticleEmitter();

	void SetManagerPosition(Vector3 pos) { m_managerPosition = pos; }
	void SetOffsetPosition(Vector3 pos) { m_offsetPosition = pos; }
	
	void Update();
	void Draw();

private:
	void CreateParticle();

	ID3D11ComputeShader* m_computeShader;

	std::shared_ptr<ParticleCompute[]> m_particle;

	ID3D11Buffer* m_particleBuffer;
	ID3D11Buffer* m_resultBuffer;
	ID3D11Buffer* m_positionBuffer;
	ID3D11Buffer* m_lifeBuffer;

	// SRV
	ID3D11ShaderResourceView* m_particleSRV;
	ID3D11ShaderResourceView* m_positionSRV;
	ID3D11ShaderResourceView* m_lifeSRV;
	// UAV
	ID3D11UnorderedAccessView* m_resultUAV;


	ID3D11ShaderResourceView* m_texture;

	ID3D11Buffer* m_vertexBuffer;

	Vector3 m_managerPosition;
	Vector3 m_offsetPosition; //エミッターマネージャーからのオフセット位置

	EmitterInitData m_initData;

	int m_particleNum;
	int m_createCount;
};