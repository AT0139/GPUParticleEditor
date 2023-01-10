#pragma once

enum class SHADER
{

};

class ShaderManager
{
public:

private:
	std::unordered_map<SHADER, ID3D11Buffer*> m_shaderMap;
};