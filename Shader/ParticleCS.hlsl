#include "ParticleHeader.hlsli"

struct CSInput
{
	uint3 groupThread : SV_GroupThreadID;
	uint3 group : SV_GroupID;
	uint groupIndex : SV_GroupIndex;
	uint3 dispatch : SV_DispatchThreadID;
};

StructuredBuffer<ParticleCompute> particle : register(t0);
RWStructuredBuffer<ParticleCompute> bufOut : register(u0);

Texture2D g_textureDepth : register(t1);


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


inline float GetDepth(float2 uv)
{
	float2 coord = float2(uv.x * 1920, uv.y * 1080);
	return g_textureDepth[coord].r;
}

[numthreads(SIZE_X, SIZE_Y, SIZE_Z)]
void main(const CSInput input)
{
	int index = input.dispatch.x;

	bufOut[index].life = particle[index].life - 1;
	if (bufOut[index].life <= 0)
	{

		bufOut[index].pos.xyz = voidPosition;
	}
	else
	{
		//速度計算
		float3 speed = particle[index].speed + info.gravity;
		float3 result = particle[index].pos + particle[index].velocity + info.velocity + speed;

		bufOut[index].velocity = particle[index].velocity;



		////スクリーンスペースコリジョン(深度バッファ)
		//{
		//	matrix wvp;
		//	wvp = mul(World, View);
		//	wvp = mul(wvp, Projection);
			
		//	float4 vpPos = mul(float4(result, 1), wvp);
		//	float2 uv = vpPos.xy / vpPos.w * 0.5 + 0.5;
		//	float gbufferDepth = GetDepth(uv);
		//	float particleDepth = vpPos.z / vpPos.w;
		//	float3 normal = float3(0, 1, 0);

			
		//	if (particleDepth > gbufferDepth)
		//	{
		//		float3 velocity = -dot(particle[index].velocity, normal) * normal * 1.99 /* 1.0 + bouciness */;
			
		//		result = particle[index].pos + velocity + info.velocity + speed;
		//	}
		//}
		
		//Position更新
		bufOut[index].pos = result;
		bufOut[index].speed = speed;
		//Size
		float t = info.maxLife - particle[index].life;
		t = t / info.maxLife;
		bufOut[index].size = lerp(info.initialSize, info.finalSize, t);
		bufOut[index].color = lerp(info.initialColor, info.finalColor, t);

	}
}