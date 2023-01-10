#pragma once

enum class SHADER
{

};

struct DrawShader
{
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* inputLayout;
};

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

private:
	std::unordered_map<SHADER, DrawShader> m_drawShaderMap;
};