#include "ParticleHeader.hlsli"

StructuredBuffer<ParticleCompute> parameter : register(t2);

void main(in VS_PARTICLE_IN In, out GS_PARTICLE_IN Out)
{
	Out.StructurePos = float4(parameter[In.instanceID].pos, 1);
	Out.Position = In.Position;
	Out.Size = parameter[In.instanceID].size;
	Out.Normal = In.Normal;
	Out.TexCoord = In.TexCoord;
	Out.Diffuse = parameter[In.instanceID].color;
	Out.Clip = parameter[In.instanceID].pos.y - voidPosition.y;
}