#include "ParticleHeader.hlsli"

struct ParticleCompute
{
	float3 pos;
	float3 speed;
	float3 velocity;
	int life;
	float2 size;
	float4 color;
};

struct CSInput
{
	uint3 groupThread : SV_GroupThreadID;
	uint3 group : SV_GroupID;
	uint groupIndex : SV_GroupIndex;
	uint3 dispatch : SV_DispatchThreadID;
};

StructuredBuffer<ParticleCompute> particle : register(t0);
RWStructuredBuffer<ParticleCompute> bufOut : register(u0);

struct Info
{
	float3 gravity;
	int maxLife;
	float3 velocity;
	float pad1;
	float2 initialSize;
	float2 finalSize;
	float4 initialColor;
	float4 finalColor;
};

cbuffer InfoBuffer : register(b7)
{
	Info info;
}


#define SIZE_X 512
#define SIZE_Y 1
#define SIZE_Z 1

[numthreads(SIZE_X, SIZE_Y, SIZE_Z)]
void main(const CSInput input)
{
	int index = input.dispatch.x;
	float3 speed = particle[index].speed + info.gravity;
	float3 result = particle[index].pos + particle[index].velocity + info.velocity + speed;

	
	bufOut[index].life = particle[index].life - 1;
	if (bufOut[index].life <= 0)
	{
		bufOut[index].pos.xyz = voidPosition;
	}
	else
	{
		bufOut[index].pos = result;
		bufOut[index].velocity = particle[index].velocity;
		bufOut[index].speed = speed;
		//Size
		float t = info.maxLife - particle[index].life;
		t = t / info.maxLife;
		bufOut[index].size = lerp(info.initialSize, info.finalSize, t);
		bufOut[index].color = lerp(info.initialColor, info.finalColor, t);
	}
}