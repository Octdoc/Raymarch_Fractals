#pragma once

#include "pch.h"
#include "camera.h"

namespace fracapp
{
	class Graphics
	{
		Microsoft::WRL::ComPtr<ID3D11Device3> m_device3D;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext3> m_context3D;
		Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;
		Windows::UI::Xaml::Controls::SwapChainPanel^ m_swapChainPanel;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState2> m_rasterizerState;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView1> m_renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader2D;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout2D;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer2D;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer2D;

		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader3D;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout3D;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer3D;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer3D;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vsBuffer3D;

		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_fractalParams;
		unsigned m_fractalParamsSize;

		D3D11_VIEWPORT m_viewport;
		D3D_FEATURE_LEVEL m_featureLevel;
		unsigned m_width, m_height;

	private:
		void CreateDevice();
		void CreateRasterizerState();
		void CreateDepthStencilState();

		void CreateWindowSizeDependentResources();
		void CreateSwapChain();
		void CreateRenderTargetView();
		void CreateViewport();

		void CreateVertexShader2D();
		void CreateVertexShader3D();
		void CreateShaderBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& buffer, unsigned size);
		void CreateQuad();
		void CreateCube();

		void SetCommonResources();
		void Set2DResources();
		void Set3DResources();
		void WriteShaderBuffer(ID3D11Buffer* buffer, const void* data, unsigned size);

	public:
		Graphics();
		void SetSwapChainPanel(Windows::UI::Xaml::Controls::SwapChainPanel^ panel);
		void Resize(unsigned width, unsigned height);
		mth::float2 ScreenSize() const;

		void SetFractalRenderer(const char* psCode, unsigned paramSize);

		void Render2D(const void* fractalParams);
		void Render3D(const Camera& camera, const void* fractalParams);
	};
}