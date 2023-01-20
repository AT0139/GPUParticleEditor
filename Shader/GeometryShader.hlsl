#include "ParticleHeader.hlsli"

#define size 1.0f

[maxvertexcount(4)]
void main(point GS_PARTICLE_IN input[1] : SV_POSITION, inout TriangleStream<PS_PARTICLE_IN> output)
{
	matrix world = World;
	matrix wvp;
	world._41 += input[0].StructurePos.x;
	world._42 += input[0].StructurePos.y;
	world._43 += input[0].StructurePos.z;

	matrix invView = inverse(View);
	invView._41 = 0.0f;
	invView._42 = 0.0f;
	invView._43 = 0.0f;
	
	wvp = mul(world, invView);
	wvp = mul(wvp, Projection);

	float4 worldPosition = mul(input[0].Position, world);
	
	float3 planeNormal = (float3) worldPosition - (float3) CameraPosition;
	planeNormal = normalize(planeNormal);

	float3 upVector = float3(0.0f, 1.0f, 0.0f);
	float3 rightVector = normalize(cross(upVector, planeNormal));
	
	rightVector = rightVector * size;
	upVector = float3(0.0f, size, 0.0f);

	//í∏ì_èÓïÒ
	float3 vert[4];
	vert[0] = input[0].Position.xyz - rightVector + upVector;
	vert[1] = input[0].Position.xyz + rightVector + upVector;
	vert[2] = input[0].Position.xyz - rightVector - upVector;
	vert[3] = input[0].Position.xyz + rightVector - upVector;

	float2 texCoord[4];
	texCoord[0] = float2(0, 0);
	texCoord[1] = float2(1, 0);
	texCoord[2] = float2(0, 1);
	texCoord[3] = float2(1, 1);

	
	PS_PARTICLE_IN outputVert;
	for (int i = 0; i < 4; i++)
	{
		outputVert.Position = mul(float4(vert[i], 1.0f), wvp);
		outputVert.WorldPosition = float4(vert[i], 0.0f);
		outputVert.TexCoord = texCoord[i];

		outputVert.Normal = float4(0, 0, 0, 0);
		outputVert.Diffuse = input[0].Diffuse;

		output.Append(outputVert);
	}
}