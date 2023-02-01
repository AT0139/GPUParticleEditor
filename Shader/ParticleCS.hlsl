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

//float3 calcViewSpacePositionFromDepth(float2 normalizedScreenPosition, int2 texelOffset)
//{
//	float2 uv;

//	// Add the texel offset to the normalized screen position
//	normalizedScreenPosition.x += (float) texelOffset.x / (float) 1920;
//	normalizedScreenPosition.y += (float) texelOffset.y / (float) 1080;

//	// Scale, bias and convert to texel range
//	uv.x = (0.5 + normalizedScreenPosition.x * 0.5) * (float) 1920;
//	uv.y = (1 - (0.5 + normalizedScreenPosition.y * 0.5)) * (float) 1080;

//	// Fetch the depth value at this point
//	float depth = g_textureDepth.Load(uint3(uv.x, uv.y, 0)).x;
	
//	// Generate a point in screen space with this depth
//	float4 viewSpacePosOfDepthBuffer;
//	viewSpacePosOfDepthBuffer.xy = normalizedScreenPosition.xy;
//	viewSpacePosOfDepthBuffer.z = depth;
//	viewSpacePosOfDepthBuffer.w = 1;

//	// Transform into view space using the inverse projection matrix
//	matrix invProj = inverse(Projection);
//	viewSpacePosOfDepthBuffer = mul(viewSpacePosOfDepthBuffer, invProj);
//	viewSpacePosOfDepthBuffer.xyz /= viewSpacePosOfDepthBuffer.w;

//	return viewSpacePosOfDepthBuffer.xyz;
//}

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
		//	wvp = View;
		//	wvp = mul(wvp, Projection);

		//	float3 particlePosition = mul(result, wvp);
			
		//	// Transform new position into view space
		//	float3 viewSpaceParticlePosition = mul(float4(particlePosition, 1), View).xyz;

		//	// Also obtain screen space position
		//	float4 screenSpaceParticlePosition = mul(float4(particlePosition, 1), mul(Projection, View));
		//	screenSpaceParticlePosition.xyz /= screenSpaceParticlePosition.w;

		//		// Get the view space position of the depth buffer
		//	float3 viewSpacePosOfDepthBuffer = calcViewSpacePositionFromDepth(screenSpaceParticlePosition.xy, int2(0, 0));

		//		// If the particle view space position is behind the depth buffer, but not by more than the collision thickness, then a collision has occurred
		//	if (viewSpaceParticlePosition.z > viewSpacePosOfDepthBuffer.z)
		//	{
		//			// Generate the surface normal. Ideally, we would use the normals from the G-buffer as this would be more reliable than deriving them
		//		float3 surfaceNormal;

		//			// Take three points on the depth buffer
		//		float3 p0 = viewSpacePosOfDepthBuffer;
		//		float3 p1 = calcViewSpacePositionFromDepth(screenSpaceParticlePosition.xy, int2(1, 0));
		//		float3 p2 = calcViewSpacePositionFromDepth(screenSpaceParticlePosition.xy, int2(0, 1));

		//			// Generate the view space normal from the two vectors
		//		float3 viewSpaceNormal = normalize(cross(p2 - p0, p1 - p0));

		//			// Transform into world space using the inverse view matrix
		//		matrix invView = inverse(View);
		//		surfaceNormal = normalize(mul(-viewSpaceNormal, invView).xyz);

		//			// The velocity is reflected in the collision plane
		//		float3 newVelocity = reflect(particle[index].velocity, surfaceNormal);

		//			// Update the velocity and apply some restitution
		//		bufOut[index].velocity = 0.3 * newVelocity;

		//			// Update the new collided position
		//		result = particle[index].pos + bufOut[index].velocity + info.velocity + speed;
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