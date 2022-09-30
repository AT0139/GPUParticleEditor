#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse:SV_Target)
{
    float4 normal = normalize(In.Normal);   //ピクセルの法線を正規化
    float light = -dot(Light.Direction.xyz, In.Normal.xyz); //光源計算をする
    
    //カメラからピクセルに向かうベクトル
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);
    
    //--------------------------
    //フォン鏡面反射
    //--------------------------
    ////光の反射ベクトルを計算
    float3 refv = reflect(Light.Direction.xyz, normal.xyz);
    refv = normalize(refv);//正規化
    
    float specular = -dot(eyev, refv);  //鏡面反射の計算
    specular = saturate(specular);      //値をサチュレート
    specular = pow(specular, 30);       //30乗

    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    //テクスチャのピクセル色を取得
    outDiffuse.rgb *= In.Diffuse.rgb * light;
    outDiffuse.a *= In.Diffuse.a;   //aに明るさは関係ないので別計算
    
    outDiffuse.rgb += specular;//スぺキュラ値をディフューズとして足しこむ
}