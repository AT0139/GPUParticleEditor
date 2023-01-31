#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp; //毎回お約束の頂点変換
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
    
	float4 worldNormal, normal; //ローカル変数を作成
  
    //頂点法線をワールド行列で回転させる(頂点と同じ回転をさせる)
	normal = float4(In.Normal.xyz, 0.0); //法線ベクトルのwを0としてコピー（平行移動しないため)
	worldNormal = mul(normal, World); //法線をワールド行列で回転する
	worldNormal = normalize(worldNormal); //回転後の法線を正規化する
    
	float light = -dot(Light.Direction.xyz, worldNormal.xyz);

	light = saturate(light);
	Out.Diffuse = light;
	Out.Diffuse.a = In.Diffuse.a;
    
	Out.Position = mul(In.Position, wvp);
	Out.Normal = worldNormal; //回転後の法線出力 In.Normalでなく回転後の法線を出力
	Out.TexCoord = In.TexCoord; //頂点のテクスチャ座標を出力

	matrix lightwvp;
	lightwvp = mul(World, Light.View);
	lightwvp = mul(lightwvp, Light.Proj);
    
	Out.ShadowPosition = mul(In.Position, lightwvp);
}