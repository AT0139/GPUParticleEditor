#pragma once

#include "Singleton.h"

enum class RASTERIZER
{
	DEFAULT,
	WIRE_FRAME,
};

struct MATERIAL
{
	Color	Ambient;
	Color	Diffuse;
	Color	Specular;
	Color	Emission;
	float		Shininess;
	float		Dummy[3];
};

struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];
	Vector4	Direction;
	Color	Diffuse;
	Color	Ambient;
};

class Renderer : public Singleton<Renderer>
{
private:
	Renderer();
	~Renderer();

	D3D_FEATURE_LEVEL m_featureLevel;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;

	ID3D11Buffer* m_pWorldBuffer;
	ID3D11Buffer* m_pViewBuffer;
	ID3D11Buffer* m_pProjectionBuffer;
	ID3D11Buffer* m_pMaterialBuffer;
	ID3D11Buffer* m_pLightBuffer;
	ID3D11Buffer* m_pCameraBuffer;

	ID3D11DepthStencilState* m_pDepthStateEnable;
	ID3D11DepthStencilState* m_pDepthStateDisable;

	ID3D11RasterizerState* m_rasterizerState;
	ID3D11RasterizerState* m_wireFrameRasterizerState;

private:
#ifdef _DEBUG
	inline static bool show_demo_window = false;
	inline static bool show_another_window = false;
	inline static bool show_hello_world = false;
	inline static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
#endif // _DEBUG
	inline static float m_ImGuiFontSize = 15;

public:
	friend class Singleton<Renderer>;

	void Init();
	void Uninit();
	void Begin();
	void End();

	void SetDepthEnable(bool Enable);
	void SetWorldViewProjection2D();
	void SetWorldMatrix(Matrix* WorldMatrix);
	void SetViewMatrix(Matrix* ViewMatrix);
	void SetProjectionMatrix(Matrix* ProjectionMatrix);
	void SetMaterial(MATERIAL material);
	void SetLight(LIGHT Light);
	void SetCameraPosition(Vector3 pos);
	void SetRasterizerState(RASTERIZER state);

	ID3D11Device* GetDevice(void) { return m_pDevice; }
	ID3D11DeviceContext* GetDeviceContext(void) { return m_pDeviceContext; }

	void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);
};
