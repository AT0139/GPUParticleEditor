#include "ParticleHeader.hlsli"

#define size 1.0f

[maxvertexcount(4)]
void main(point PS_PARTICLE_IN input[1] : SV_POSITION, inout TriangleStream<PS_PARTICLE_IN> output)
{
	float3 planeNormal = input[0].WorldPosition - CameraPosition;
	//planeNormal.y = 0.0f;
	planeNormal = normalize(planeNormal);

	float3 upVector = float3(0.0f, 1.0f, 0.0f);
	float3 rightVector = normalize(cross(planeNormal, upVector));

	rightVector = rightVector * size;
	upVector = float3(0, size, 0);

	//í∏ì_èÓïÒ
	float3 vert[4];
	vert[0] = input[0].WorldPosition.xyz - rightVector + upVector;
	vert[1] = input[0].WorldPosition.xyz + rightVector + upVector;
	vert[2] = input[0].WorldPosition.xyz - rightVector - upVector;
	vert[3] = input[0].WorldPosition.xyz + rightVector - upVector;

	float2 texCoord[4];
	texCoord[0] = float2(0, 0);
	texCoord[1] = float2(1, 0);
	texCoord[2] = float2(0, 1);
	texCoord[3] = float2(1, 1);
	
	
	matrix world = World;
	matrix wvp;
	//world._41 += input[0].StructurePos.x;
	//world._42 += input[0].StructurePos.y;
	//world._43 += input[0].StructurePos.z;
	
	wvp = mul(world, View);
	wvp = mul(wvp, Projection);
	
	PS_PARTICLE_IN outputVert;
	for (int i = 0; i < 4; i++)
	{
		outputVert.Position = mul(float4(vert[i], 1.0f), wvp);
		outputVert.StructurePos = outputVert.Position;
		outputVert.WorldPosition = float4(vert[i], 0.0f);
		outputVert.TexCoord = texCoord[i];

		outputVert.Normal = float4(0, 0, 0, 0);
		outputVert.Diffuse = float4(1, 1, 1, 1);

		output.Append(outputVert);
	}
}