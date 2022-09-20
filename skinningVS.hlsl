#include "skinning.hlsl"

void main(in VSInput input,out VSOutput output)
{
    float4 localPos = float4(input.Position, 1.0f);

    float4x4 skinTransform = float4x4(1.0f, 0.0f, 0.0f, 0.0f,
		                               0.0f, 1.0f, 0.0f, 0.0f,
									   0.0f, 0.0f, 1.0f, 0.0f,
									   0.0f, 0.0f, 0.0f, 1.0f);

	// スキニング処理
    skinTransform += Bones[input.SkinIndex.x] * input.SkinWeight.x;
    skinTransform += Bones[input.SkinIndex.y] * input.SkinWeight.y;
    skinTransform += Bones[input.SkinIndex.z] * input.SkinWeight.z;
    skinTransform += Bones[input.SkinIndex.w] * input.SkinWeight.w;

	// 位置座標を変換
    float4 transPos = mul(skinTransform, localPos);
    float4 worldPos = mul(World, transPos);
    float4 viewPos = mul(View, worldPos);
    float4 projPos = mul(Proj, viewPos);

	// 法線ベクトルを変換
    float3 transNormal = mul((float3x3) skinTransform, input.Normal);
    float3 worldNormal = mul((float3x3) World, transNormal);

	// 出力値設定
    output.Position = projPos;
    output.TexCoord = input.TexCoord;
    output.Normal = worldNormal;
}