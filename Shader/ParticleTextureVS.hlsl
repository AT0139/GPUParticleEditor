#include "ParticleHeader.hlsli"

StructuredBuffer<float3> position: register(t2);

void main(in VS_PARTICLE_IN In, out PS_PARTICLE_IN Out)
{
	matrix world = World;
	world._41 += position[In.instanceID].x;
	world._42 += position[In.instanceID].y;
	world._43 += position[In.instanceID].z;

	matrix wvp;
	wvp = mul(world, View);
	wvp = mul(wvp, Projection);
	
	Out.Position = mul(In.Position, wvp);
	Out.StructurePos = float4(position[In.instanceID], 0.0f);
	Out.WorldPosition = mul(In.Position, world);
	Out.Normal = mul(In.Normal, world);
	Out.TexCoord = In.TexCoord;
	Out.Diffuse = In.Diffuse * Material.Diffuse;
	Out.Diffuse.a = In.Diffuse.a * Material.Diffuse.a;
}