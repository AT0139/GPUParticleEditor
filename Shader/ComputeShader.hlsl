
struct ParticleCompute
{
	float3 pos;
	float3 vel;
	float life;
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

#define SIZE_X 256
#define SIZE_Y 1
#define SIZE_Z 1

[numthreads(SIZE_X, SIZE_Y, SIZE_Z)]
void main(const CSInput input)
{
	int index = input.dispatch.x;

	float3 result = particle[index].pos + particle[index].vel;

	bufOut[index].pos = result;
	bufOut[index].life = particle[index].life - 1.0f;
	bufOut[index].vel = particle[index].vel;
}