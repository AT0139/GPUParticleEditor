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
	Color	diffuse;
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
	Color	diffuse;
	Color	Ambient;

	Matrix view;
	Matrix proj;
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
	ID3D11RasterizerState* m_depthRasterizerState;

	Viewport m_viewport;

	ID3D11DepthStencilView* m_depthDSV;
	ID3D11ShaderResourceView* m_depthSRV;
	

private:
#ifdef _DEBUG
	inline static bool show_demo_window = false;
	inline static bool show_another_window = false;
	inline static bool show_hello_world = false;
	inline static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
#endif // _DEBUG
	inline static float m_ImGuiFontSize = 20;

public:
	friend class Singleton<Renderer>;

	void Init();
	void Uninit();
	void Begin();
	void BeginDepth();
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

	Viewport GetViewport();
	ID3D11ShaderResourceView* GetShadowDepthTexture() { return m_depthSRV; }

	ID3D11Device* GetDevice(void) { return m_pDevice; }
	ID3D11DeviceContext* GetDeviceContext(void) { return m_pDeviceContext; }

	void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);
	void CreateComputeShader(ID3D11ComputeShader** computeShader, const char* FileName);
	void CreateGeometryShader(ID3D11GeometryShader** geometryShader, const char* FileName);

	void CreateConstantBuffer(ID3D11Buffer** pBuffer, D3D11_SUBRESOURCE_DATA* initData, UINT size, UINT stride, UINT slot);
	void CreateStructuredBuffer(UINT elementSize, UINT count, void* initData, ID3D11Buffer** ppBuffer,bool useMap = false);
	void CreateBufferSRV(ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSrv);
	void CreateBufferUAV(ID3D11Buffer* pBuffer, ID3D11UnorderedAccessView** ppUrv);
	ID3D11Buffer* CreateAndCopyToBuffer(ID3D11Buffer* buffer);
	void UpdateConstantBuffer(ID3D11Resource* buffer, void* data);
};
