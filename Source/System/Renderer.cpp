#include "main.h"
#include "renderer.h"
#include <io.h>

Renderer::Renderer()
	: m_featureLevel(D3D_FEATURE_LEVEL_11_0)
	, m_pDevice(nullptr)
	, m_pDeviceContext(nullptr)
	, m_pSwapChain(nullptr)
	, m_pRenderTargetView(nullptr)
	, m_pDepthStencilView(nullptr)
	, m_pWorldBuffer(nullptr)
	, m_pViewBuffer(nullptr)
	, m_pProjectionBuffer(nullptr)
	, m_pMaterialBuffer(nullptr)
	, m_pLightBuffer(nullptr)
	, m_pCameraBuffer(nullptr)
	, m_pDepthStateEnable(nullptr)
	, m_pDepthStateDisable(nullptr)
{}

Renderer::~Renderer()
{}

void Renderer::Init()
{
	HRESULT hr = S_OK;


	IDXGIFactory* factory;
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&factory));

	IDXGIAdapter* hpAdapter = nullptr;
	IDXGIAdapter* adapter;
	size_t videoMemSize = 0;
	for (int i = 0; factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; i++)
	{
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);
		if (videoMemSize < desc.DedicatedVideoMemory)
		{
			videoMemSize = desc.DedicatedVideoMemory;
			hpAdapter = adapter;
		}
	}

	factory->Release();


	// デバイス、スワップチェーン作成
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = SCREEN_WIDTH;
	swapChainDesc.BufferDesc.Height = SCREEN_HEIGHT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = GetWindow();
	swapChainDesc.SampleDesc.Count = 1;		//MSAA設定
	swapChainDesc.SampleDesc.Quality = 0;	//MSAA設定
	swapChainDesc.Windowed = TRUE;

	UINT creationFlags = 0;
#if defined(_DEBUG)
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	hr = D3D11CreateDeviceAndSwapChain(
		hpAdapter,
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_pSwapChain,
		&m_pDevice,
		&m_featureLevel,
		&m_pDeviceContext);

	// レンダーターゲットビュー作成
	ID3D11Texture2D* renderTarget = NULL;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTarget);
	m_pDevice->CreateRenderTargetView(renderTarget, NULL, &m_pRenderTargetView);
	renderTarget->Release();

	// デプスステンシルバッファ作成
	ID3D11Texture2D* depthStencile = NULL;
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = swapChainDesc.BufferDesc.Width;
	textureDesc.Height = swapChainDesc.BufferDesc.Height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D16_UNORM;
	textureDesc.SampleDesc = swapChainDesc.SampleDesc;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	m_pDevice->CreateTexture2D(&textureDesc, NULL, &depthStencile);

	// デプスステンシルビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = textureDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;	//MSAA設定
	depthStencilViewDesc.Flags = 0;
	m_pDevice->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &m_pDepthStencilView);
	depthStencile->Release();

	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	//ビューポート設定
	m_viewport.width = (FLOAT)SCREEN_WIDTH;
	m_viewport.height = (FLOAT)SCREEN_HEIGHT;
	m_viewport.minDepth = 0.0f;
	m_viewport.maxDepth = 1.0f;

	m_pDeviceContext->RSSetViewports(1, m_viewport.Get11());

	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;

	m_pDevice->CreateRasterizerState(&rasterizerDesc, &m_rasterizerState);

	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;

	m_pDevice->CreateRasterizerState(&rasterizerDesc, &m_wireFrameRasterizerState);

	m_pDeviceContext->RSSetState(m_rasterizerState);

	// ブレンドステート設定
	D3D11_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = TRUE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11BlendState* blendState = NULL;
	m_pDevice->CreateBlendState(&blendDesc, &blendState);
	m_pDeviceContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);

	// デプスステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStateEnable);//深度有効ステート

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStateDisable);//深度無効ステート

	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStateEnable, NULL);

	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC; //異方性フィルタリング
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	samplerDesc.MipLODBias = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState = NULL;
	m_pDevice->CreateSamplerState(&samplerDesc, &samplerState);

	m_pDeviceContext->PSSetSamplers(0, 1, &samplerState);

	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(Matrix);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	m_pDevice->CreateBuffer(&bufferDesc, NULL, &m_pWorldBuffer);
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pWorldBuffer);
	m_pDeviceContext->GSSetConstantBuffers(0, 1, &m_pWorldBuffer);
	m_pDeviceContext->CSSetConstantBuffers(0, 1, &m_pWorldBuffer);

	m_pDevice->CreateBuffer(&bufferDesc, NULL, &m_pViewBuffer);
	m_pDeviceContext->VSSetConstantBuffers(1, 1, &m_pViewBuffer);
	m_pDeviceContext->GSSetConstantBuffers(1, 1, &m_pViewBuffer);
	m_pDeviceContext->CSSetConstantBuffers(1, 1, &m_pViewBuffer);

	m_pDevice->CreateBuffer(&bufferDesc, NULL, &m_pProjectionBuffer);
	m_pDeviceContext->VSSetConstantBuffers(2, 1, &m_pProjectionBuffer);
	m_pDeviceContext->GSSetConstantBuffers(2, 1, &m_pProjectionBuffer);
	m_pDeviceContext->CSSetConstantBuffers(2, 1, &m_pProjectionBuffer);

	bufferDesc.ByteWidth = sizeof(MATERIAL);

	m_pDevice->CreateBuffer(&bufferDesc, NULL, &m_pMaterialBuffer);
	m_pDeviceContext->VSSetConstantBuffers(3, 1, &m_pMaterialBuffer);

	bufferDesc.ByteWidth = sizeof(LIGHT);

	m_pDevice->CreateBuffer(&bufferDesc, NULL, &m_pLightBuffer);
	m_pDeviceContext->VSSetConstantBuffers(4, 1, &m_pLightBuffer);
	m_pDeviceContext->PSSetConstantBuffers(4, 1, &m_pLightBuffer);

	bufferDesc.ByteWidth = sizeof(Vector4);

	m_pDevice->CreateBuffer(&bufferDesc, NULL, &m_pCameraBuffer);
	m_pDeviceContext->PSSetConstantBuffers(5, 1, &m_pCameraBuffer);
	m_pDeviceContext->GSSetConstantBuffers(5, 1, &m_pCameraBuffer);

	// ライト初期化
	LIGHT light{};
	light.Enable = true;
	light.Direction = Vector4(1.0f, -1.0f, 1.0f, 0.0f);
	light.Direction = XMVector4Normalize(light.Direction);
	light.Ambient = Color(0.2f, 0.2f, 0.2f, 1.0f);
	light.diffuse = Color(2.0f, 2.0f, 2.0f, 1.0f);
	SetLight(light);

	// マテリアル初期化
	MATERIAL material{};
	material.diffuse = Color(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = Color(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	//ジオメトリプリミティブ作成

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	ImGui_ImplWin32_Init(GetWindow());
	ImGui_ImplDX11_Init(m_pDevice, m_pDeviceContext);

	//	Fonts
	io.Fonts->AddFontFromFileTTF("Source/imgui/misc/fonts/Roboto-Medium.ttf", m_ImGuiFontSize);

	//style
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.0f, 1.0f, 220.0f / 250.0f, 1.0f));
	//ホバー色
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.3f, 0.85f, 0.875f, 0.4f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.0f, 1.0f, 0.9f, 0.7f));

	//ウィンドウボーダーサイズ
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);

	//ウィンドウのタイトルのアラインメント
	ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5f, 0.5f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20.0f, 20.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20.0f, 3.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 10.0f));

	{
		ID3D11Texture2D* depthTexture = NULL;
		D3D11_TEXTURE2D_DESC td;
		ZeroMemory(&td, sizeof(td));
		td.Width = swapChainDesc.BufferDesc.Width;
		td.Height = swapChainDesc.BufferDesc.Height;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		td.Format = DXGI_FORMAT_R32_TYPELESS;

		hr = m_pDevice->CreateTexture2D(&td, NULL, &depthTexture);
		if (FAILED(hr))
			assert(nullptr);

		//レンダーターゲットビュー
		D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
		ZeroMemory(&dsvd, sizeof(dsvd));
		dsvd.Format = DXGI_FORMAT_D32_FLOAT;
		dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		hr = m_pDevice->CreateDepthStencilView(depthTexture, &dsvd, &m_depthDSV);
		if (FAILED(hr))
			assert(nullptr);

		//シェーダーリソースビュー作成
		D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
		SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;//ピクセルフォーマットは32BitFLOAT型
		SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		SRVDesc.Texture2D.MipLevels = 1;
		hr = m_pDevice->CreateShaderResourceView(depthTexture, &SRVDesc, &m_depthSRV);
		if (FAILED(hr))
			assert(nullptr);
		depthTexture->Release();
	}
}


void Renderer::Uninit()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	m_pWorldBuffer->Release();
	m_pViewBuffer->Release();
	m_pProjectionBuffer->Release();
	m_pLightBuffer->Release();
	m_pMaterialBuffer->Release();
	m_pCameraBuffer->Release();

	m_pDeviceContext->ClearState();
	m_pRenderTargetView->Release();
	m_pSwapChain->Release();
	m_pDeviceContext->Release();
	m_pDevice->Release();

	m_depthDSV->Release();
	m_depthSRV->Release();
}

void Renderer::Begin()
{
	m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	float clearColor[4] = { 0.4f, 0.4f, 0.4f, 1.0f };
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, clearColor);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::BeginDepth()
{
	m_pDeviceContext->OMSetRenderTargets(0, NULL, m_depthDSV);
	m_pDeviceContext->ClearDepthStencilView(m_depthDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::End()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	m_pSwapChain->Present(1, 0);
}

void Renderer::SetDepthEnable(bool Enable)
{
	if (Enable)
		m_pDeviceContext->OMSetDepthStencilState(m_pDepthStateEnable, NULL);
	else
		m_pDeviceContext->OMSetDepthStencilState(m_pDepthStateDisable, NULL);
}

void Renderer::SetWorldViewProjection2D()
{
	Matrix world = XMMatrixIdentity();

	world = XMMatrixTranspose(world);

	m_pDeviceContext->UpdateSubresource(m_pWorldBuffer, 0, NULL, &world, 0, 0);

	Matrix view = XMMatrixIdentity();
	view = XMMatrixTranspose(view);
	m_pDeviceContext->UpdateSubresource(m_pViewBuffer, 0, NULL, &view, 0, 0);

	Matrix projection;
	projection = XMMatrixOrthographicOffCenterLH(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	projection = XMMatrixTranspose(projection);
	m_pDeviceContext->UpdateSubresource(m_pProjectionBuffer, 0, NULL, &projection, 0, 0);
}

void Renderer::SetWorldMatrix(Matrix* WorldMatrix)
{
	Matrix world = XMMatrixTranspose(*WorldMatrix);
	m_pDeviceContext->UpdateSubresource(m_pWorldBuffer, 0, NULL, &world, 0, 0);
}

void Renderer::SetViewMatrix(Matrix* ViewMatrix)
{
	Matrix view = XMMatrixTranspose(*ViewMatrix);
	m_pDeviceContext->UpdateSubresource(m_pViewBuffer, 0, NULL, &view, 0, 0);
}

void Renderer::SetProjectionMatrix(Matrix* ProjectionMatrix)
{
	Matrix projection = XMMatrixTranspose(*ProjectionMatrix);
	m_pDeviceContext->UpdateSubresource(m_pProjectionBuffer, 0, NULL, &projection, 0, 0);
}

void Renderer::SetMaterial(MATERIAL material)
{
	m_pDeviceContext->UpdateSubresource(m_pMaterialBuffer, 0, NULL, &material, 0, 0);
}

void Renderer::SetLight(LIGHT Light)
{
	Light.view = XMMatrixTranspose(Light.view);
	Light.proj = XMMatrixTranspose(Light.proj);
	m_pDeviceContext->UpdateSubresource(m_pLightBuffer, 0, NULL, &Light, 0, 0);
}

void Renderer::SetCameraPosition(Vector3 pos)
{
	m_pDeviceContext->UpdateSubresource(m_pCameraBuffer, 0, NULL, &pos, 0, 0);
}

void Renderer::SetRasterizerState(RASTERIZER state)
{
	switch (state)
	{
	case RASTERIZER::DEFAULT:
		m_pDeviceContext->RSSetState(m_rasterizerState);
		break;
	case RASTERIZER::WIRE_FRAME:
		m_pDeviceContext->RSSetState(m_wireFrameRasterizerState);
		break;
	default:
		break;
	}
}

Viewport Renderer::GetViewport()
{
	return m_viewport;
}

void Renderer::CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName)
{
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_pDevice->CreateVertexShader(buffer, fsize, NULL, VertexShader);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	m_pDevice->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		VertexLayout);

	delete[] buffer;
}

void Renderer::CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName)
{
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_pDevice->CreatePixelShader(buffer, fsize, NULL, PixelShader);

	delete[] buffer;
}

void Renderer::CreateComputeShader(ID3D11ComputeShader** computeShader, const char* FileName)
{
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	Renderer::GetInstance().GetDevice()->CreateComputeShader(buffer, fsize, nullptr, computeShader);

	delete[] buffer;
}

void Renderer::CreateGeometryShader(ID3D11GeometryShader** geometryShader, const char* FileName)
{
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	Renderer::GetInstance().GetDevice()->CreateGeometryShader(buffer, fsize, nullptr, geometryShader);

	delete[] buffer;
}

void Renderer::CreateConstantBuffer(ID3D11Buffer** pBuffer, D3D11_SUBRESOURCE_DATA* initData, UINT size, UINT stride, UINT slot)
{
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = size;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = stride;

	auto hr = m_pDevice->CreateBuffer(&bufferDesc, initData, pBuffer);
	if (FAILED(hr))
		assert(nullptr);
	m_pDeviceContext->VSSetConstantBuffers(slot, 1, pBuffer);
}

void Renderer::CreateStructuredBuffer(UINT elementSize, UINT count, void* initData, ID3D11Buffer** ppBuffer, bool useMap)
{
	*ppBuffer = nullptr;

	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	bufferDesc.ByteWidth = elementSize * count;
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	bufferDesc.StructureByteStride = elementSize;
	if (useMap)
	{
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		bufferDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	}

	auto hr = m_pDevice->CreateBuffer(&bufferDesc, nullptr, ppBuffer);
	if (FAILED(hr))
		assert(nullptr);
}

void Renderer::CreateBufferSRV(ID3D11Buffer* pBuffer, ID3D11ShaderResourceView** ppSrv)
{
	D3D11_BUFFER_DESC descBuf = {};
	pBuffer->GetDesc(&descBuf);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	srvDesc.Buffer.FirstElement = 0;

	if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	{
		//生バッファ
		srvDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		srvDesc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
		srvDesc.BufferEx.NumElements = descBuf.ByteWidth / 4;
	}
	else
	{
		//structBuffer
		srvDesc.Format = DXGI_FORMAT_UNKNOWN;
		srvDesc.Buffer.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
	}

	auto hr = m_pDevice->CreateShaderResourceView(pBuffer, &srvDesc, ppSrv);
	if (FAILED(hr))
		assert(nullptr);
}

void Renderer::CreateBufferUAV(ID3D11Buffer* pBuffer, ID3D11UnorderedAccessView** ppUrv)
{
	D3D11_BUFFER_DESC descBuf = {};
	pBuffer->GetDesc(&descBuf);

	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc = {};
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	UAVDesc.Buffer.FirstElement = 0;

	if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	{
		UAVDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		UAVDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
		UAVDesc.Buffer.NumElements = descBuf.ByteWidth / 4;
	}
	else if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
	{
		UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
		UAVDesc.Buffer.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
	}

	auto hr = m_pDevice->CreateUnorderedAccessView(pBuffer, &UAVDesc, ppUrv);
	if (FAILED(hr))
		assert(nullptr);
}

ID3D11Buffer* Renderer::CreateAndCopyToBuffer(ID3D11Buffer* buffer)
{
	ID3D11Buffer* debugbuf = nullptr;

	D3D11_BUFFER_DESC desc = {};
	buffer->GetDesc(&desc);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.MiscFlags = 0;
	auto hr = m_pDevice->CreateBuffer(&desc, NULL, &debugbuf);

	m_pDeviceContext->CopyResource(debugbuf, buffer);

	return debugbuf;
}

void Renderer::UpdateConstantBuffer(ID3D11Resource* buffer, void* data)
{
	m_pDeviceContext->UpdateSubresource(buffer, 0, NULL, &data, 0, 0);
}
