﻿#pragma once


struct ParticleCompute
{
	Vector3 pos;
	Vector3 vel;
	int life;
};

struct EmitterInitData
{
	Vector2 size;
	Vector3 direction;
	int life;
};

class ParticleEmitter
{
public:
	ParticleEmitter(EmitterInitData initData);
	~ParticleEmitter();

	void SetManagerPosition(Vector3 pos) { m_managerPosition = pos; }
	
	void Update();
	void Draw();
	void Load(const wchar_t* filePath);

private:
	ID3D11ComputeShader* m_computeShader;

	std::shared_ptr<ParticleCompute[]> m_particle;

	ID3D11Buffer* m_particleBuffer;
	ID3D11Buffer* m_resultBuffer;
	ID3D11Buffer* m_positionBuffer;

	// SRV
	ID3D11ShaderResourceView* m_particleSRV;
	ID3D11ShaderResourceView* m_positionSRV;
	// UAV
	ID3D11UnorderedAccessView* m_resultUAV;


	ID3D11ShaderResourceView* m_texture;

	ID3D11Buffer* m_vertexBuffer;
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_vertexLayout;

	Vector3 m_managerPosition;
	Vector3 m_offsetPosition; //エミッターマネージャーからのオフセット位置
	
};