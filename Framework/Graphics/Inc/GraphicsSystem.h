#pragma once

#include "Color.h"

namespace MoonEngine::Graphics
{
	class GraphicsSystem final
	{
	public:
		static void StaticInitialize(HWND window, bool fullscreen);
		static void StaticTerminate();
		static GraphicsSystem* Get();

		GraphicsSystem() = default;
		~GraphicsSystem();

		// this is singleton so we must get rid of copy and move constructors
		GraphicsSystem(const GraphicsSystem&) = delete;
		GraphicsSystem(GraphicsSystem&&) = delete;
		GraphicsSystem& operator=(const GraphicsSystem&) = delete;
		GraphicsSystem& operator=(GraphicsSystem&&) = delete;

		void Initialize(HWND window, bool fullscreen);
		void Terminate();

		void BeginRender();
		void EndRender();

		void ToggleFullScreen();
		void Resize(uint32_t width, uint32_t height);

		void ResetRenderTarget();
		void ResetViewport();

		void SetClearColor(const Color& color);
		void SetVSync(bool vSync);

		uint32_t GetBackBufferWidth() const;
		uint32_t GetBackBufferHeight() const;
		float GetBackBufferAspectRatio() const;

		// need to create a resource from the graphics system
		ID3D11Device* GetDevice();
		// need to communicate with the graphics sytem with a created resource
		ID3D11DeviceContext* GetContext();

	private:
		static LRESULT CALLBACK GraphicsSystemMessageHandler(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);

		ID3D11Device* m_D3DDevice = nullptr;
		ID3D11DeviceContext* m_immediateContext = nullptr;

		IDXGISwapChain* m_swapChain = nullptr;
		ID3D11RenderTargetView* m_renderTargetView = nullptr;

		ID3D11Texture2D* m_depthStencilBuffer = nullptr;
		ID3D11DepthStencilView* m_depthStencilView = nullptr;

		DXGI_SWAP_CHAIN_DESC m_swapChainDesc{};
		D3D11_VIEWPORT m_viewport{};

		Color m_clearColor = Colors::Black;
		UINT m_vSync = 1;

	};
}