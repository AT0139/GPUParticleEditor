#include "common.hlsl"

Texture2D g_texture : register(t0);
Texture2D g_envTexture : register(t1);
SamplerState g_samplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4 normal = normalize(In.Normal);
	
	//カメラからピクセルに向かうベクトル
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev);

	float3 refv = reflect(Light.Direction.xyz, normal.xyz);
	refv = normalize(refv);

	float2 envTexCoord;
	envTexCoord.x = -refv.x * 0.5f + 0.5f;
	envTexCoord.y = -refv.y * 0.5f + 0.5f;

	float4 envTex = g_envTexture.SampleBias(g_samplerState, envTexCoord, 0.0f);
	float4 baseTex = g_texture.Sample(g_samplerState, In.TexCoord);
	
	//テクスチャのピクセル色を取得
	outDiffuse.rgb = envTex.rgb + baseTex.rgb;
	outDiffuse.a = baseTex.a * In.Diffuse.a; //aに明るさは関係ないので別計算
}