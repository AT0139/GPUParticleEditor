
#include "ParticleHeader.hlsli"


Texture2D		g_Texture : register( t0 );
SamplerState	g_SamplerState : register( s0 );


void main(in PS_PARTICLE_IN In, out float4 outDiffuse : SV_Target)
{
	clip(abs(In.Clip) - 0.01f);
	
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);	
	outDiffuse *= In.Diffuse;
}
