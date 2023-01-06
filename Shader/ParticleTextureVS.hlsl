#include "common.hlsl"

StructuredBuffer<float3> position: register(t2);

void main(in VS_IN In, out PS_IN Out)
{
	matrix world = World;
	world._41 += position[In.instanceID].x;
	world._42 += position[In.instanceID].y;
	world._43 += position[In.instanceID].z;

	matrix wvp;
	wvp = mul(world, View);
	wvp = mul(wvp, Projection);
	
	//In.Position.xyz += position[In.instanceID];
	
	Out.Position = mul(In.Position, wvp);
	Out.TexCoord = In.TexCoord;
	Out.Diffuse = In.Diffuse;
}