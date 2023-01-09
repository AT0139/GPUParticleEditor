#include "common.hlsl"

StructuredBuffer<float3> position: register(t2);
StructuredBuffer<int> life : register(t3);


void main(in VS_IN In, out PS_IN Out)
{
	matrix world = World;
	if (life[In.instanceID] <= 0)
	{
		world._41 = -100.0f;
		world._42 = -100.0f;
		world._43 = -100.0f;

	}
	else
	{
		world._41 += position[In.instanceID].x;
		world._42 += position[In.instanceID].y;
		world._43 += position[In.instanceID].z;
	}
	matrix wvp;
	wvp = mul(world, View);
	wvp = mul(wvp, Projection);
	
	//In.Position.xyz += position[In.instanceID];
	
	Out.Position = mul(In.Position, wvp);
	Out.TexCoord = In.TexCoord;
	Out.Diffuse = In.Diffuse;
	Out.Diffuse.a = In.Diffuse.a;
}