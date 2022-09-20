//-----------------------------------------------------------------------
// File : Skinning.hlsl
// Desc : Skinning Shader
// Copyright(c) Project Asura. All right reserved.
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// History
//-----------------------------------------------------------------------
// <---> 2011/08/15 [Pocol] 新規作成.
// <End>


//-----------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------
#define MAX_BONES 48


//-----------------------------------------------------------------------
// Constant Buffer and Global Variables
//-----------------------------------------------------------------------

// 行列バッファ
cbuffer CBufMatrix : register( b0 )
{
	float4x4 World : packoffset( c0 );
	float4x4 View  : packoffset( c4 );
	float4x4 Proj  : packoffset( c8 );
};

// マテリアルバッファ
cbuffer CBufMaterial : register( b1 )
{
	float3 Diffuse  : packoffset( c0 );
	float  Alpha    : packoffset( c0.w );
	float3 Specular : packoffset( c1 );
	float  Power    : packoffset( c1.w );
	float3 Emissive : packoffset( c2 );
};

// ベクトルバッファ
cbuffer CBufVector : register( b2 )
{
	float3 LightDir : packoffset( c0 );
	float3 ViewDir  : packoffset( c1 );
};

// スキニングバッファ
cbuffer CBufSkinning : register( b3 )
{
	float4x4 Bones[ MAX_BONES ];
};

// デカールテクスチャ
Texture2D    DecalTex : register( t0 );

// デカールサンプラーステート
SamplerState DecalSmp : register( s0 );


/////////////////////////////////////////////////////////////////////////
// VSInput structure
/////////////////////////////////////////////////////////////////////////
struct VSInput
{
	float3 Position   : POSITION;
	float3 Normal     : NORMAL;
	float2 TexCoord   : TEXCOORD;
	uint4  SkinIndex  : SKIN_INDEX;
	float4 SkinWeight : SKIN_WEIGHT;
};


/////////////////////////////////////////////////////////////////////////
// VSOutput structure
/////////////////////////////////////////////////////////////////////////
struct VSOutput
{
	float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
	float3 Normal   : TEXCOORD1;
};


/////////////////////////////////////////////////////////////////////////
// PSOutput structure
/////////////////////////////////////////////////////////////////////////
struct PSOutput
{
	float4 Color : SV_TARGET;
};


//-----------------------------------------------------------------------
//! @brief スキニング処理
//-----------------------------------------------------------------------
VSOutput main( VSInput input )
{
	VSOutput output = (VSOutput)0;

	float4 localPos = float4( input.Position, 1.0f );

	float4x4 skinTransform = float4x4( 1.0f, 0.0f, 0.0f, 0.0f,
		                               0.0f, 1.0f, 0.0f, 0.0f,
									   0.0f, 0.0f, 1.0f, 0.0f,
									   0.0f, 0.0f, 0.0f, 1.0f );

	// スキニング処理
	skinTransform += Bones[ input.SkinIndex.x ] * input.SkinWeight.x;
	skinTransform += Bones[ input.SkinIndex.y ] * input.SkinWeight.y;
	skinTransform += Bones[ input.SkinIndex.z ] * input.SkinWeight.z;
	skinTransform += Bones[ input.SkinIndex.w ] * input.SkinWeight.w;

	// 位置座標を変換
	float4 transPos = mul( skinTransform, localPos );
	float4 worldPos = mul( World, transPos );
	float4 viewPos  = mul( View, worldPos );
	float4 projPos  = mul( Proj, viewPos );

	// 法線ベクトルを変換
	float3 transNormal = mul( (float3x3)skinTransform, input.Normal );
	float3 worldNormal = mul( (float3x3)World, transNormal );

	// 出力値設定
	output.Position = projPos;
	output.TexCoord = input.TexCoord;
	output.Normal   = worldNormal;

	return output;
}

//-----------------------------------------------------------------------
//! @brief ライティング処理
//-----------------------------------------------------------------------
PSOutput main( VSOutput input )
{
	PSOutput output = (PSOutput)0;

	// 正規化
	float3 L = normalize( LightDir );
    float3 N = normalize( input.Normal );
    float3 E = normalize( ViewDir );
    float3 H = normalize( L + E );

	// マテリアルカラーを算出
    float3 diffuse  = Diffuse  * 0.75f * max( dot( L, N ), 0 );
    float3 specular = Specular * 0.25f * pow( max( dot( N, H ), 0 ), Power );
	float3 emissive = Emissive * 0.15f;
	output.Color = float4( ( diffuse + specular + emissive ), Alpha );

	// テクスチャカラーを乗算
	float4 decal = DecalTex.Sample( DecalSmp, input.TexCoord );
	output.Color *= decal;
 
	return output;
}