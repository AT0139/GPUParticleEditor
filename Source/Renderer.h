#pragma once

#include "Singleton.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"


struct VERTEX_3D
{
    D3DXVECTOR3 Position;
    D3DXVECTOR3 Normal;
    D3DXVECTOR4 Diffuse;
    D3DXVECTOR2 TexCoord;
};



struct MATERIAL
{
	D3DXCOLOR	Ambient;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Specular;
	D3DXCOLOR	Emission;
	float		Shininess;
	float		Dummy[3];
};



struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];
	D3DXVECTOR4	Direction;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Ambient;
};



class Renderer : public Singleton<Renderer>
{
private:
	Renderer();
	~Renderer();

	D3D_FEATURE_LEVEL m_featureLevel;
	
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain*	m_pSwapChain;
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
	void SetWorldMatrix(D3DXMATRIX* WorldMatrix);
	void SetViewMatrix(D3DXMATRIX* ViewMatrix);
	void SetProjectionMatrix(D3DXMATRIX* ProjectionMatrix);
	void SetMaterial(MATERIAL material);
	void SetLight(LIGHT Light);
	void SetCameraPosition(D3DXVECTOR3 pos);

	ID3D11Device* GetDevice( void ){ return m_pDevice; }
	ID3D11DeviceContext* GetDeviceContext( void ){ return m_pDeviceContext; }

	void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);
};
