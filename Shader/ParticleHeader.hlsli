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




struct LIGHT
{
	bool Enable;
	bool3 Dummy;
	float4 Direction;
	float4 Diffuse;
	float4 Ambient;
};

cbuffer LightBuffer : register(b4)
{
	LIGHT Light;
}

cbuffer CameraBuffer : register(b5)
{
	float4 CameraPosition;
}

cbuffer ParameterBuffer : register(b6)
{
	float4 Parameter;
}

struct VS_PARTICLE_IN
{
	float4 Position : POSITION0;
	float4 Normal : NORMAL0;
	float4 Diffuse : COLOR0;
	float2 TexCoord : TEXCOORD0;
	uint instanceID : SV_InstanceID;
};

struct PS_PARTICLE_IN
{
	float4 Position : SV_POSITION;
	float4 StructurePos : POSITION1;
	float4 WorldPosition : POSITION0;
	float4 Diffuse : COLOR0;
	float4 Normal : NORMAL0;
	float2 TexCoord : TEXCOORD0;
};
