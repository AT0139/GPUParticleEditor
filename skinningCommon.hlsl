#define MAX_BONES 48

// �s��o�b�t�@
cbuffer WorldBuffer : register(b0)
{
    matrix World;
}
cbuffer ViewBuffer : register(b1)
{
    matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
    matrix Projection;
}


// �}�e���A���o�b�t�@
struct MATERIAL
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float4 Emission;
    float Shininess;
    float3 Dummy;
};

cbuffer MaterialBuffer : register(b3)
{
    MATERIAL Material;
}

// �x�N�g���o�b�t�@
cbuffer CBufVector : register(b2)
{
    float3 LightDir : packoffset(c0);
    float3 ViewDir : packoffset(c1);
};

// �X�L�j���O�o�b�t�@
cbuffer CBufSkinning : register(b3)
{
    float4x4 Bones[MAX_BONES];
};

// �f�J�[���e�N�X�`��
Texture2D DecalTex : register(t0);

// �f�J�[���T���v���[�X�e�[�g
SamplerState DecalSmp : register(s0);


/////////////////////////////////////////////////////////////////////////
// VSInput structure
/////////////////////////////////////////////////////////////////////////
struct VSInput
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD;
    uint4 SkinIndex : SKIN_INDEX;
    float4 SkinWeight : SKIN_WEIGHT;
};


/////////////////////////////////////////////////////////////////////////
// VSOutput structure
/////////////////////////////////////////////////////////////////////////
struct VSOutput
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
    float3 Normal : TEXCOORD1;
};


/////////////////////////////////////////////////////////////////////////
// PSOutput structure
/////////////////////////////////////////////////////////////////////////
struct PSOutput
{
    float4 Color : SV_TARGET;
};
