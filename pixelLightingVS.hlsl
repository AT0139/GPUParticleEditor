#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
    matrix wvp; //���񂨖񑩂̒��_�ϊ�
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    Out.Position = mul(In.Position, wvp);
    
    float4 worldNormal, normal; //���[�J���ϐ����쐬
    
    //���_�@�������[���h�s��ŉ�]������(���_�Ɠ�����]��������)
    normal = float4(In.Normal.xyz, 0.0); //�@���x�N�g����w��0�Ƃ��ăR�s�[�i���s�ړ����Ȃ�����)
    worldNormal = mul(normal, World); //�@�������[���h�s��ŉ�]����
    worldNormal = normalize(worldNormal); //��]��̖@���𐳋K������
    Out.Normal = worldNormal; //��]��̖@���o�� In.Normal�łȂ���]��̖@�����o��
    
    
    Out.Diffuse = In.Diffuse;
    Out.TexCoord = In.TexCoord; //���_�̃e�N�X�`�����W���o��
    Out.WorldPosition = mul(In.Position, World);
}