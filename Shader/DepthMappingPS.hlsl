#include "common.hlsl"

Texture2D g_Texture : register(t0);
Texture2D g_textureShadowDepth : register(t1);
SamplerState g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
	outDiffuse *= In.Diffuse;
    
	In.ShadowPosition.xyz /= In.ShadowPosition.w;
	In.ShadowPosition.x = In.ShadowPosition.x * 0.5f + 0.5f;
	In.ShadowPosition.y = -In.ShadowPosition.y * 0.5f + 0.5f;
    
	float depth = g_textureShadowDepth.Sample(g_SamplerState, In.ShadowPosition.xy);
    
	if (depth < In.ShadowPosition.z - 0.01) //0.01はZファイティング補正値（後述）
	{
		outDiffuse.rgb *= 0.5; //色を暗くする
	}
}