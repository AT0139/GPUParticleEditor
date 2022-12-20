#include "common.hlsl"

Texture2D g_texture : register(t0);
Texture2D g_normalTexture : register(t1);
SamplerState g_samplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
	float4 normalMap = g_normalTexture.Sample(g_samplerState, In.TexCoord);
	normalMap = normalMap * 2 - 1.0f;
	
	float4 normal = 0;
	normal.x = -normalMap.r;
	normal.y = normalMap.b;
	normal.z = normalMap.g;
	normal.w = 0.0f;

	float light = -dot(normal.xyz, Light.Direction.xyz); //光源計算をする
	
	//カメラからピクセルに向かうベクトル
	float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
	eyev = normalize(eyev);

	//ブリンフォン
	//光の反射ベクトルを計算
	float3 halfv = eyev + Light.Direction.xyz;
	halfv = normalize(halfv); //正規化
	
	float specular = -dot(halfv, normal.xyz); //鏡面反射の計算
	specular = saturate(specular); //値をサチュレート
	specular = pow(specular, 240);

	outDiffuse = g_texture.Sample(g_samplerState, In.TexCoord);
	//テクスチャのピクセル色を取得
	outDiffuse.rgb *= In.Diffuse.rgb * light;
	outDiffuse.a *= In.Diffuse.a; //aに明るさは関係ないので別計算
	
	outDiffuse.rgb += specular; //スぺキュラ値をディフューズとして足しこむ
}