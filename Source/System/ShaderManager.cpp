#include "ShaderManager.h"
#include "Renderer.h"


ShaderManager::ShaderManager()
{
	DrawShader shader;

	Renderer::GetInstance().CreateVertexShader(&shader.vertexShader, &shader.inputLayout, "Shader/unlitTextureVS.cso");
	Renderer::GetInstance().CreatePixelShader(&shader.pixelShader, "Shader/unlitTexturePS.cso");
	m_drawShaderMap[SHADER_TYPE::UNLIT] = shader;

	Renderer::GetInstance().CreateVertexShader(&shader.vertexShader, &shader.inputLayout, "Shader/vertexLightingVS.cso");
	Renderer::GetInstance().CreatePixelShader(&shader.pixelShader, "Shader/vertexLightingPS.cso");
	m_drawShaderMap[SHADER_TYPE::VERTEX_LIGHTING] = shader;

	Renderer::GetInstance().CreateVertexShader(&shader.vertexShader, &shader.inputLayout, "Shader/pixelLightingVS.cso");
	Renderer::GetInstance().CreatePixelShader(&shader.pixelShader, "Shader/pixelLightingPS.cso");
	m_drawShaderMap[SHADER_TYPE::PIXEL_LIGHTING] = shader;

	Renderer::GetInstance().CreateVertexShader(&shader.vertexShader, &shader.inputLayout, "Shader/EnvMappingVS.cso");
	Renderer::GetInstance().CreatePixelShader(&shader.pixelShader, "Shader/EnvMappingPS.cso");
	m_drawShaderMap[SHADER_TYPE::ENV_MAPPING] = shader;

	Renderer::GetInstance().CreateVertexShader(&shader.vertexShader, &shader.inputLayout, "Shader/NormalMappingVS.cso");
	Renderer::GetInstance().CreatePixelShader(&shader.pixelShader, "Shader/NormalMappingPS.cso");
	m_drawShaderMap[SHADER_TYPE::NORMAL_MAPPING] = shader;

	Renderer::GetInstance().CreateVertexShader(&shader.vertexShader, &shader.inputLayout, "Shader/ParticleTextureVS.cso");
	Renderer::GetInstance().CreatePixelShader(&shader.pixelShader, "Shader/ParticleTexturePS.cso");
	m_drawShaderMap[SHADER_TYPE::PARTICLE] = shader;

	Renderer::GetInstance().CreateVertexShader(&shader.vertexShader, &shader.inputLayout, "Shader/DepthMappingVS.cso");
	Renderer::GetInstance().CreatePixelShader(&shader.pixelShader, "Shader/DepthMappingPS.cso");
	m_drawShaderMap[SHADER_TYPE::DEPTH] = shader;
}

ShaderManager::~ShaderManager()
{
	for (auto it : m_drawShaderMap)
	{
		it.second.inputLayout->Release();
		it.second.pixelShader->Release();
		it.second.vertexShader->Release();
	}
}

void ShaderManager::Set(SHADER_TYPE type)
{
	Renderer::GetInstance().GetDeviceContext()->IASetInputLayout(m_drawShaderMap[type].inputLayout);
	Renderer::GetInstance().GetDeviceContext()->VSSetShader(m_drawShaderMap[type].vertexShader, NULL, 0);
	Renderer::GetInstance().GetDeviceContext()->PSSetShader(m_drawShaderMap[type].pixelShader, NULL, 0);
}
