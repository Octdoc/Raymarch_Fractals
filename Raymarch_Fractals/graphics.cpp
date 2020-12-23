#include "pch.h"
#include "graphics.h"
#include <windows.ui.xaml.media.dxinterop.h>
#include <d3dcompiler.h>

namespace fracapp
{
	void Graphics::CreateDevice()
	{
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1
		};
		Microsoft::WRL::ComPtr<ID3D11Device> device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;

		HRESULT hr = D3D11CreateDevice(
			nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_BGRA_SUPPORT, featureLevels,
			ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &device, &m_featureLevel, &context);

		if (FAILED(hr))
			ThrowIfFailed(D3D11CreateDevice(
				nullptr, D3D_DRIVER_TYPE_WARP, 0, D3D11_CREATE_DEVICE_BGRA_SUPPORT, featureLevels,
				ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &device, &m_featureLevel, &context));

		ThrowIfFailed(device.As(&m_device3D));
		ThrowIfFailed(context.As(&m_context3D));
	}
	void Graphics::CreateRasterizerState()
	{
		D3D11_RASTERIZER_DESC2 rasterizerDesc{};
		rasterizerDesc.AntialiasedLineEnable = false;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.DepthBias = 0;
		rasterizerDesc.DepthBiasClamp = 0.0f;
		rasterizerDesc.SlopeScaledDepthBias = 0.0f;
		rasterizerDesc.DepthClipEnable = true;
		rasterizerDesc.FrontCounterClockwise = false;
		rasterizerDesc.MultisampleEnable = false;
		rasterizerDesc.ScissorEnable = false;
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.ForcedSampleCount = 0;
		rasterizerDesc.ConservativeRaster = D3D11_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		ThrowIfFailed(m_device3D->CreateRasterizerState2(&rasterizerDesc, &m_rasterizerState));
	}
	void Graphics::CreateDepthStencilState()
	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;
		depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		ThrowIfFailed(m_device3D->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState));
		m_context3D->OMSetDepthStencilState(m_depthStencilState.Get(), 0);
	}
	void Graphics::CreateWindowSizeDependentResources()
	{
		ID3D11RenderTargetView* nullViews[] = { nullptr };
		m_context3D->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
		m_renderTargetView = nullptr;
		m_depthStencilView = nullptr;
		m_context3D->Flush1(D3D11_CONTEXT_TYPE_ALL, nullptr);

		CreateSwapChain();
		CreateRenderTargetView();
		CreateViewport();

		m_swapChainPanel->Dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::High, ref new Windows::UI::Core::DispatchedHandler([this]()
			{
				Microsoft::WRL::ComPtr<ISwapChainPanelNative> panelNative;
				ThrowIfFailed(reinterpret_cast<IUnknown*>(m_swapChainPanel)->QueryInterface(IID_PPV_ARGS(&panelNative)));
				ThrowIfFailed(panelNative->SetSwapChain(m_swapChain.Get()));
			}, Platform::CallbackContext::Any));
	}
	void Graphics::CreateSwapChain()
	{
		if (m_swapChain)
		{
			ThrowIfFailed(m_swapChain->ResizeBuffers(2, m_width, m_height, DXGI_FORMAT_B8G8R8A8_UNORM, 0));
		}
		else
		{
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
			swapChainDesc.Width = max(1, m_width);
			swapChainDesc.Height = max(1, m_height);
			swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			swapChainDesc.Stereo = false;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = 2;
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			swapChainDesc.Flags = 0;
			swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
			swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

			Microsoft::WRL::ComPtr<IDXGIDevice3> dxgiDevice;
			ThrowIfFailed(m_device3D.As(&dxgiDevice));

			Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
			ThrowIfFailed(dxgiDevice->GetAdapter(&dxgiAdapter));

			Microsoft::WRL::ComPtr<IDXGIFactory4> dxgiFactory;
			ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory)));

			Microsoft::WRL::ComPtr<IDXGISwapChain1> swapChain;
			ThrowIfFailed(dxgiFactory->CreateSwapChainForComposition(m_device3D.Get(), &swapChainDesc, nullptr, &swapChain));

			ThrowIfFailed(swapChain.As(&m_swapChain));

			ThrowIfFailed(dxgiDevice->SetMaximumFrameLatency(1));
		}
	}
	void Graphics::CreateRenderTargetView()
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D1> backBuffer;
		ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer)));
		ThrowIfFailed(m_device3D->CreateRenderTargetView1(backBuffer.Get(), nullptr, &m_renderTargetView));

		CD3D11_TEXTURE2D_DESC1 depthStencilDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, max(1, m_width), max(1, m_height), 1, 1, D3D11_BIND_DEPTH_STENCIL);
		Microsoft::WRL::ComPtr<ID3D11Texture2D1> depthStencil;
		ThrowIfFailed(m_device3D->CreateTexture2D1(&depthStencilDesc, nullptr, &depthStencil));
		CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
		ThrowIfFailed(m_device3D->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, &m_depthStencilView));
	}
	void Graphics::CreateViewport()
	{
		m_viewport = CD3D11_VIEWPORT(0.0f, 0.0f, static_cast<float>(m_width), static_cast<float>(m_height));
	}
	Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(const std::string& shaderCode, const char* entry, const char* target)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> shaderBuffer;
		Microsoft::WRL::ComPtr<ID3DBlob> errorMessage;
		HRESULT hr = D3DCompile(shaderCode.c_str(), shaderCode.length(), nullptr, nullptr, nullptr, entry, target, 0, 0, &shaderBuffer, &errorMessage);
		if (FAILED(hr))
		{
			std::string error = (char*)errorMessage->GetBufferPointer();
			ThrowIfFailed(hr);
		}
		return shaderBuffer;
	}
	void Graphics::CreateVertexShader2D()
	{
		static const char* code = R"(
struct VertexInputType
{
	float3 position : POSITION;
};
struct PixelInputType
{
	float4 windowPosition : SV_POSITION;
};
PixelInputType main(VertexInputType input)
{
	PixelInputType output;
	output.windowPosition = float4(input.position, 1.0f);
	return output;
}
)";
		const char* target = nullptr;
		switch (m_featureLevel)
		{
		case D3D_FEATURE_LEVEL_10_0:
			target = "vs_4_0";
			break;
		case D3D_FEATURE_LEVEL_10_1:
			target = "vs_4_1";
			break;
		case D3D_FEATURE_LEVEL_11_0:
		case D3D_FEATURE_LEVEL_11_1:
		case D3D_FEATURE_LEVEL_12_0:
		case D3D_FEATURE_LEVEL_12_1:
			target = "vs_5_0";
			break;
		default:
			target = "vs_4_0";
		}
		Microsoft::WRL::ComPtr<ID3DBlob> byteCode = CompileShader(code, "main", target);
		ThrowIfFailed(m_device3D->CreateVertexShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, &m_vertexShader2D));
		D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[1];
		inputLayoutDesc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		ThrowIfFailed(m_device3D->CreateInputLayout(inputLayoutDesc, ARRAYSIZE(inputLayoutDesc), byteCode->GetBufferPointer(), byteCode->GetBufferSize(), &m_inputLayout2D));
	}
	void Graphics::CreateVertexShader3D()
	{
		static const char* code = R"(
cbuffer MatrixBuffer
{
	matrix cb_cameraMatrix;
};
struct VertexInputType
{
	float3 position : POSITION;
};
struct PixelInputType
{
	float4 windowPosition : SV_POSITION;
	float3 position : POSITION;
};
PixelInputType main(VertexInputType input)
{
	PixelInputType output;
	output.position = input.position;
	output.windowPosition = mul(float4(input.position, 1.0f), cb_cameraMatrix);
	return output;
}
)";
		const char* target = nullptr;
		switch (m_featureLevel)
		{
		case D3D_FEATURE_LEVEL_10_0:
			target = "vs_4_0";
			break;
		case D3D_FEATURE_LEVEL_10_1:
			target = "vs_4_1";
			break;
		case D3D_FEATURE_LEVEL_11_0:
		case D3D_FEATURE_LEVEL_11_1:
		case D3D_FEATURE_LEVEL_12_0:
		case D3D_FEATURE_LEVEL_12_1:
			target = "vs_5_0";
			break;
		default:
			target = "vs_4_0";
		}
		Microsoft::WRL::ComPtr<ID3DBlob> byteCode = CompileShader(code, "main", target);
		ThrowIfFailed(m_device3D->CreateVertexShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, &m_vertexShader3D));
		D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[1];
		inputLayoutDesc[0] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		ThrowIfFailed(m_device3D->CreateInputLayout(inputLayoutDesc, ARRAYSIZE(inputLayoutDesc), byteCode->GetBufferPointer(), byteCode->GetBufferSize(), &m_inputLayout3D));
	}
	void Graphics::CreateShaderBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& buffer, unsigned size)
	{
		D3D11_BUFFER_DESC bufferDesc{};
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = 0;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.ByteWidth = size;
		ThrowIfFailed(m_device3D->CreateBuffer(&bufferDesc, nullptr, &buffer));
	}
	void Graphics::CreateQuad()
	{
		mth::float3 vertices[] = {
			mth::float3(-1.0f, -1.0f,  1.0f),
			mth::float3(-1.0f,  1.0f,  1.0f),
			mth::float3( 1.0f,  1.0f,  1.0f),
			mth::float3( 1.0f, -1.0f,  1.0f)
		};
		unsigned indices[] = {
			0, 2, 1, 0, 3, 2
		};

		D3D11_BUFFER_DESC bufferDesc{};
		D3D11_SUBRESOURCE_DATA subResourceData{};

		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.ByteWidth = sizeof(vertices);
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		subResourceData.pSysMem = vertices;
		ThrowIfFailed(m_device3D->CreateBuffer(&bufferDesc, &subResourceData, &m_vertexBuffer2D));

		bufferDesc.ByteWidth = sizeof(indices);
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		subResourceData.pSysMem = indices;
		ThrowIfFailed(m_device3D->CreateBuffer(&bufferDesc, &subResourceData, &m_indexBuffer2D));
	}
	void Graphics::CreateCube()
	{
		mth::float3 vertices[] = {
			mth::float3(-1.0f, -1.0f, -1.0f),
			mth::float3(-1.0f,  1.0f, -1.0f),
			mth::float3(-1.0f,  1.0f,  1.0f),
			mth::float3(-1.0f, -1.0f,  1.0f),
			mth::float3( 1.0f,  1.0f,  1.0f),
			mth::float3( 1.0f, -1.0f,  1.0f),
			mth::float3( 1.0f,  1.0f, -1.0f),
			mth::float3( 1.0f, -1.0f, -1.0f)
		};
		unsigned indices[] = {
			0, 2, 1, 0, 3, 2, 3, 4, 2, 3, 5, 4, 5, 6, 4, 5, 7, 6 ,7 ,1 ,6 ,7 ,0, 1, 3, 7, 5, 3, 0, 7, 4, 1, 2, 4, 6, 1
		};

		D3D11_BUFFER_DESC bufferDesc{};
		D3D11_SUBRESOURCE_DATA subResourceData{};

		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		bufferDesc.ByteWidth = sizeof(vertices);
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		subResourceData.pSysMem = vertices;
		ThrowIfFailed(m_device3D->CreateBuffer(&bufferDesc, &subResourceData, &m_vertexBuffer3D));

		bufferDesc.ByteWidth = sizeof(indices);
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		subResourceData.pSysMem = indices;
		ThrowIfFailed(m_device3D->CreateBuffer(&bufferDesc, &subResourceData, &m_indexBuffer3D));
	}
	void Graphics::SetCommonResources()
	{
		ID3D11RenderTargetView* rendeTargets[1] = { m_renderTargetView.Get() };
		m_context3D->RSSetState(m_rasterizerState.Get());
		m_context3D->PSSetShader(m_pixelShader.Get(), nullptr, 0);
		m_context3D->RSSetViewports(1, &m_viewport);
		m_context3D->OMSetRenderTargets(1, rendeTargets, m_depthStencilView.Get());
		m_context3D->PSSetConstantBuffers(0, 1, m_fractalParams.GetAddressOf());
		m_context3D->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		float color[] = { 0.4f, 0.25f, 0.14f, 1.0f };
		m_context3D->ClearRenderTargetView(m_renderTargetView.Get(), color);
		m_context3D->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
	void Graphics::Set2DResources()
	{
		m_context3D->VSSetShader(m_vertexShader2D.Get(), nullptr, 0);

		UINT stride = sizeof(mth::float3);
		UINT offset = 0;
		m_context3D->IASetInputLayout(m_inputLayout2D.Get());
		m_context3D->IASetVertexBuffers(0, 1, m_vertexBuffer2D.GetAddressOf(), &stride, &offset);
		m_context3D->IASetIndexBuffer(m_indexBuffer2D.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
	void Graphics::Set3DResources()
	{
		m_context3D->VSSetShader(m_vertexShader3D.Get(), nullptr, 0);
		m_context3D->VSSetConstantBuffers(0, 1, m_vsBuffer3D.GetAddressOf());

		UINT stride = sizeof(mth::float3);
		UINT offset = 0;
		m_context3D->IASetInputLayout(m_inputLayout3D.Get());
		m_context3D->IASetVertexBuffers(0, 1, m_vertexBuffer3D.GetAddressOf(), &stride, &offset);
		m_context3D->IASetIndexBuffer(m_indexBuffer3D.Get(), DXGI_FORMAT_R32_UINT, 0);
	}
	void Graphics::WriteShaderBuffer(ID3D11Buffer* buffer, const void* data, unsigned size)
	{
		D3D11_MAPPED_SUBRESOURCE resource;
		if (SUCCEEDED(m_context3D->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource)))
		{
			memcpy(resource.pData, data, size);
			m_context3D->Unmap(buffer, 0);
		}
	}

	Graphics::Graphics() :
		m_fractalParamsSize(0),
		m_viewport(),
		m_featureLevel(D3D_FEATURE_LEVEL_9_1),
		m_width(1),
		m_height(1)
	{
		CreateDevice();
		CreateRasterizerState();
		CreateDepthStencilState();

		CreateVertexShader2D();
		CreateVertexShader3D();
		CreateShaderBuffer(m_vsBuffer3D, sizeof(mth::float4x4));
		CreateQuad();
		CreateCube();
	}
	void Graphics::SetSwapChainPanel(Windows::UI::Xaml::Controls::SwapChainPanel^ panel)
	{
		m_swapChainPanel = panel;
		m_width = max(1, static_cast<unsigned>(panel->Width));
		m_height = max(1, static_cast<unsigned>(panel->Height));
		CreateWindowSizeDependentResources();
	}
	void Graphics::Resize(unsigned width, unsigned height)
	{
		m_width = max(1, width);
		m_height = max(1, height);
		CreateWindowSizeDependentResources();
	}
	mth::float2 Graphics::ScreenSize() const
	{
		return mth::float2(static_cast<float>(m_width), static_cast<float>(m_height));
	}
	void Graphics::SetFractalRenderer(const char* psCode, unsigned paramSize)
	{
		const char* target = nullptr;
		switch (m_featureLevel)
		{
		case D3D_FEATURE_LEVEL_10_0:
			target = "ps_4_0";
			break;
		case D3D_FEATURE_LEVEL_10_1:
			target = "ps_4_1";
			break;
		case D3D_FEATURE_LEVEL_11_0:
		case D3D_FEATURE_LEVEL_11_1:
		case D3D_FEATURE_LEVEL_12_0:
		case D3D_FEATURE_LEVEL_12_1:
			target = "ps_5_0";
			break;
		default:
			target = "ps_4_0";
		}
		Microsoft::WRL::ComPtr<ID3DBlob> byteCode = CompileShader(psCode, "main", target);
		ThrowIfFailed(m_device3D->CreatePixelShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, &m_pixelShader));
		m_fractalParamsSize = paramSize;
		CreateShaderBuffer(m_fractalParams, m_fractalParamsSize);
	}
	void Graphics::Render2D(const void* fractalParams)
	{
		if (m_swapChain)
		{
			SetCommonResources();
			Set2DResources();
			WriteShaderBuffer(m_fractalParams.Get(), fractalParams, m_fractalParamsSize);
			m_context3D->DrawIndexed(6, 0, 0);
			m_swapChain->Present(1, 0);
		}
	}
	void Graphics::Render3D(const Camera& camera, const void* fractalParams)
	{
		if (m_swapChain)
		{
			mth::float4x4 cameraMatrix = camera.CameraMatrix();
			SetCommonResources();
			Set3DResources();
			WriteShaderBuffer(m_vsBuffer3D.Get(), &cameraMatrix, sizeof(cameraMatrix));
			WriteShaderBuffer(m_fractalParams.Get(), fractalParams, m_fractalParamsSize);
			m_context3D->DrawIndexed(36, 0, 0);
			m_swapChain->Present(1, 0);
		}
	}
}