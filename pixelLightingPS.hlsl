#include "common.hlsl"

Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void main(in PS_IN In, out float4 outDiffuse:SV_Target)
{
    float4 normal = normalize(In.Normal);   //�s�N�Z���̖@���𐳋K��
    float light = -dot(Light.Direction.xyz, In.Normal.xyz); //�����v�Z������
    
    //�J��������s�N�Z���Ɍ������x�N�g��
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);
    
    //--------------------------
    //�t�H�����ʔ���
    //--------------------------
    ////���̔��˃x�N�g�����v�Z
    float3 refv = reflect(Light.Direction.xyz, normal.xyz);
    refv = normalize(refv);//���K��
    
    float specular = -dot(eyev, refv);  //���ʔ��˂̌v�Z
    specular = saturate(specular);      //�l���T�`�����[�g
    specular = pow(specular, 30);       //30��

    outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
    //�e�N�X�`���̃s�N�Z���F���擾
    outDiffuse.rgb *= In.Diffuse.rgb * light;
    outDiffuse.a *= In.Diffuse.a;   //a�ɖ��邳�͊֌W�Ȃ��̂ŕʌv�Z
    
    outDiffuse.rgb += specular;//�X�؃L�����l���f�B�t���[�Y�Ƃ��đ�������
}