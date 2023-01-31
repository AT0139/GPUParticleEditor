#pragma once

//todo ; ParFrame,ParUnit,スポーン確立(間引き)
enum class SPAWN_TYPE
{
	PAR_SECOND,
	//PAR_FRAME,
	BURST
	//PAR_UNIT
};

enum class ADD_VELOCITY_TYPE
{
	NONE,
	IN_CONE,
};

struct EmitterInitData
{
	Vector2 size			= Vector2(1.0f, 1.0f);
	int life				= 30;
	const wchar_t* filePath	= L"Asset\\Texture\\WhiteBloom.png";
	int maxNum				= 10000;
	SPAWN_TYPE spawnType	= SPAWN_TYPE::PAR_SECOND;
	Color color				= Color(1.0f,1.0f,1.0f,1.0f);
	Vector3 gravity			= {};
};

struct BufferInfo
{
	Vector3 gravity;
	int maxLife;
	Vector3 velocity;
	float pad1;

	Vector2 initialSize		= Vector2(1.0f, 1.0f);
	Vector2 finalSize		= Vector2(1.0f, 1.0f);

	Vector4 initialColor	= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	Vector4 finalColor		= Vector4(1.0f, 1.0f, 1.0f, 1.0f);
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
	void SetInitialSize(Vector2 size);
	void SetFinalSize(Vector2 size);
	void SetInitialColor(Vector4 color);
	void SetFinalColor(Vector4 color);
	void SetGravity(Vector3 power);
	void SetLife(int life);
	void SetVelocity(Vector3 vel, ADD_VELOCITY_TYPE type);
	void SetSpawnRate(float rate);
	void SetSpawnType(SPAWN_TYPE type) { m_spawnType = type; }
	void SetCreateInterval(float interval) { m_createInterval = interval; }
	void SetCreateOnceNum(int num) { m_createOnceNum = num; }

private:
	struct ParticleParameter
	{
		Vector3 pos;
		Vector2 size;
		Vector4 color;
	};

	//コンピュートシェーダで仕様する構造体
	struct ParticleCompute
	{
		Vector3 pos;
		Vector3 speed;
		Vector3 velocity;

		int life;
		Vector2 size;
		Vector4 color;
	};



	void CreateParticle(int createNum);

	ID3D11ComputeShader* m_computeShader;
	ID3D11GeometryShader* m_geometryShader;

	std::shared_ptr<ParticleCompute[]> m_particle; //todo : notSharedPtr

	ID3D11Buffer* m_particleComputeBuffer;
	ID3D11Buffer* m_resultBuffer;
	ID3D11Buffer* m_parameterBuffer;
	ID3D11Buffer* m_gravityBuffer;

	// SRV
	ID3D11ShaderResourceView* m_particleSRV;
	ID3D11ShaderResourceView* m_parameterSRV;
	// UAV
	ID3D11UnorderedAccessView* m_resultUAV;


	ID3D11ShaderResourceView* m_texture;

	ID3D11Buffer* m_vertexBuffer;

	Vector3 m_managerPosition;
	Vector3 m_offsetPosition; //エミッターマネージャーからのオフセット位置
	BufferInfo m_bufferInfo;
	EmitterInitData m_initData;

	int m_particleNum;
	float m_createCount;
	bool m_gravity;
	ADD_VELOCITY_TYPE m_velocityType;
	SPAWN_TYPE m_spawnType;
	float m_spawnRate;
	int m_createOnceNum;
	float m_createInterval;
};