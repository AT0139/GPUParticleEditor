#pragma once

#include "Singleton.h"

enum class SHADER_TYPE
{
	UNLIT,
	VERTEX_LIGHTING,
	PIXEL_LIGHTING,
	ENV_MAPPING,
	NORMAL_MAPPING,
	PARTICLE,
	DEPTH,
};

struct DrawShader
{
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* inputLayout;
};

class ShaderManager : public Singleton<ShaderManager>
{
public:
	friend class Singleton<ShaderManager>;

	ShaderManager();
	~ShaderManager();

	void Set(SHADER_TYPE type);
private:
	std::unordered_map<SHADER_TYPE, DrawShader> m_drawShaderMap;
};