#include "skinning.hlsl"

void main(in VSOutput input,out PSOutput output:SV_Target)
{
	// ���K��
    float3 L = normalize(LightDir);
    float3 N = normalize(input.Normal);
    float3 E = normalize(ViewDir);
    float3 H = normalize(L + E);

	// �}�e���A���J���[���Z�o
    float3 diffuse = Diffuse * 0.75f * max(dot(L, N), 0);
    float3 specular = Specular * 0.25f * pow(max(dot(N, H), 0), Power);
    float3 emissive = Emissive * 0.15f;
    output.Color = float4((diffuse + specular + emissive), Alpha);

	// �e�N�X�`���J���[����Z
    float4 decal = DecalTex.Sample(DecalSmp, input.TexCoord);
    output.Color *= decal;
}