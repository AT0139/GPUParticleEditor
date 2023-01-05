#pragma once

#include "QuadParticle.h"


struct ParticleCompute
{
	Vector3 pos;
	Vector3 vel;
	float life;
}; 
static const UINT PARTICLE_AMOUNT = 100;


class ParticleEmitter
{
public:
	ParticleEmitter();
	~ParticleEmitter();

	void SetManagerPosition(Vector3 pos) { m_managerPosition = pos; }
	
	void Update();
	void Draw();
	void Load(const wchar_t* filePath);

private:
	ID3D11ComputeShader* m_computeShader;

	ParticleCompute m_particle[PARTICLE_AMOUNT];

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

	int m_particleAmount;



	Vector3 m_managerPosition;
	Vector3 m_offsetPosition; //エミッターマネージャーからのオフセット位置
	
};