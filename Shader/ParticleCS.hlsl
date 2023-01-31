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


float3 calcViewSpacePositionFromDepth(float2 normalizedScreenPosition, int2 texelOffset)
{
	float2 uv;

	// Add the texel offset to the normalized screen position
	normalizedScreenPosition.x += (float) texelOffset.x / (float) 1920;
	normalizedScreenPosition.y += (float) texelOffset.y / (float) 1080;

	// Scale, bias and convert to texel range
	uv.x = (0.5 + normalizedScreenPosition.x * 0.5) * (float) 1920;
	uv.y = (1 - (0.5 + normalizedScreenPosition.y * 0.5)) * (float) 1080;

	// Fetch the depth value at this point
	float depth = g_textureDepth.Load(uint3(uv.x, uv.y, 0)).x;
	
	// Generate a point in screen space with this depth
	float4 viewSpacePosOfDepthBuffer;
	viewSpacePosOfDepthBuffer.xy = normalizedScreenPosition.xy;
	viewSpacePosOfDepthBuffer.z = depth;
	viewSpacePosOfDepthBuffer.w = 1;

	// Transform into view space using the inverse projection matrix
	matrix invProj = inverse(Projection);
	viewSpacePosOfDepthBuffer = mul(viewSpacePosOfDepthBuffer, invProj);
	viewSpacePosOfDepthBuffer.xyz /= viewSpacePosOfDepthBuffer.w;

	return viewSpacePosOfDepthBuffer.xyz;
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

		
		{
			float3 viewSpacePosition = mul(float4(result, 1), View).xyz;
			float4 screenSpacePosition = mul(float4(result, 1), mul(View, Projection));
			screenSpacePosition.xyz /= screenSpacePosition.w;

			if(screenSpacePosition.x > -1&& screenSpacePosition.x<1&&
				screenSpacePosition.y > -1&&screenSpacePosition.y <1)
			{
				float3 viewSpacePositionOfDepth = calcViewSpacePositionFromDepth(screenSpacePosition.xy, int2(0, 0));

				if(viewSpacePosition.z > viewSpacePositionOfDepth.z)
				{
					
					bufOut[index].life = 0;

				}
			}
		}
		
		//Position更新
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