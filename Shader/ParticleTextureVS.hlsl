#include "ParticleHeader.hlsli"

StructuredBuffer<float3> position: register(t2);

void main(in VS_PARTICLE_IN In, out GS_PARTICLE_IN Out)
{
	Out.StructurePos = float4(position[In.instanceID], 1);
	Out.Position = In.Position;
	Out.Normal = In.Normal;
	Out.TexCoord = In.TexCoord;
	Out.Diffuse = In.Diffuse * Material.Diffuse;
	Out.Diffuse.a = In.Diffuse.a * Material.Diffuse.a;
}