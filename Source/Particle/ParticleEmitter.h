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
	int life				= 30;
	const wchar_t* filePath	= L"Asset\\Texture\\WhiteBloom.png";
	int maxNum				= 100000;
	int createOnceNum		= 100;
	int createInterval		= 10;
	Color color				= Color(1.0f,1.0f,1.0f,1.0f);
	Vector3 gravity			= {};
};

struct BufferInfo
{
	Vector3 gravity;
	float p1;
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

	EmitterInitData* GetEmitterData() { return &m_initData; }
	void SetSize(Vector2 size);
	void SetGravity(Vector3 power);

private:
	void CreateParticle();

	ID3D11ComputeShader* m_computeShader;
	ID3D11GeometryShader* m_geometryShader;

	std::shared_ptr<ParticleCompute[]> m_particle;

	ID3D11Buffer* m_particleBuffer;
	ID3D11Buffer* m_resultBuffer;
	ID3D11Buffer* m_positionBuffer;
	ID3D11Buffer* m_gravityBuffer;
	ID3D11Buffer* m_WVPBuffer;

	// SRV
	ID3D11ShaderResourceView* m_particleSRV;
	ID3D11ShaderResourceView* m_positionSRV;
	// UAV
	ID3D11UnorderedAccessView* m_resultUAV;


	ID3D11ShaderResourceView* m_texture;

	ID3D11Buffer* m_vertexBuffer;

	Vector3 m_managerPosition;
	Vector3 m_offsetPosition; //エミッターマネージャーからのオフセット位置

	EmitterInitData m_initData;

	int m_particleNum;
	int m_createCount;
	bool m_gravity;
};