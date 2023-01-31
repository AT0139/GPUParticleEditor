#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
	matrix wvp; //���񂨖񑩂̒��_�ϊ�
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);
    
	float4 worldNormal, normal; //���[�J���ϐ����쐬
  
    //���_�@�������[���h�s��ŉ�]������(���_�Ɠ�����]��������)
	normal = float4(In.Normal.xyz, 0.0); //�@���x�N�g����w��0�Ƃ��ăR�s�[�i���s�ړ����Ȃ�����)
	worldNormal = mul(normal, World); //�@�������[���h�s��ŉ�]����
	worldNormal = normalize(worldNormal); //��]��̖@���𐳋K������
    
	float light = -dot(Light.Direction.xyz, worldNormal.xyz);

	light = saturate(light);
	Out.Diffuse = light;
	Out.Diffuse.a = In.Diffuse.a;
    
	Out.Position = mul(In.Position, wvp);
	Out.Normal = worldNormal; //��]��̖@���o�� In.Normal�łȂ���]��̖@�����o��
	Out.TexCoord = In.TexCoord; //���_�̃e�N�X�`�����W���o��

	matrix lightwvp;
	lightwvp = mul(World, Light.View);
	lightwvp = mul(lightwvp, Light.Proj);
    
	Out.ShadowPosition = mul(In.Position, lightwvp);
}