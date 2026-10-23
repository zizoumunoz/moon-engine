#include "Precompiled.h"
#include "GraphicsSystem.h"

using namespace MoonEngine;
using namespace MoonEngine::Graphics;

// doing this no name namespace makes it so only this file can access what is in here
namespace
{
	std::unique_ptr<GraphicsSystem> s_GraphicsSystem;
	Core::WindowMessageHandler s_windowMessageHandler;
}

LRESULT MoonEngine::Graphics::GraphicsSystem::GraphicsSystemMessageHandler(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (s_GraphicsSystem != nullptr)
	{
		switch (msg)
		{
		case WM_SIZE:
		{
			const uint32_t width = static_cast<uint32_t>(LOWORD(lParam));
			const uint32_t height = static_cast<uint32_t>(HIWORD(lParam));
			s_GraphicsSystem->Resize(width, height);
			break;

		}
		default:
			break;
		}
	}
	return s_windowMessageHandler.ForwardMessage(window, msg, wParam, lParam);
}

void MoonEngine::Graphics::GraphicsSystem::StaticInitialize(HWND window, bool fullscreen)
{
	ASSERT(s_GraphicsSystem == nullptr, "GraphicsSystem: is already initialized");
	s_GraphicsSystem = std::make_unique<GraphicsSystem>();
	s_GraphicsSystem->Initialize(window, fullscreen);
}

void MoonEngine::Graphics::GraphicsSystem::StaticTerminate()
{
	if (s_GraphicsSystem != nullptr)
	{
		s_GraphicsSystem->Terminate();
		s_GraphicsSystem.reset();
	}
}

GraphicsSystem* Graphics::GraphicsSystem::Get()
{
	ASSERT(s_GraphicsSystem != nullptr, "GraphicsSystem: is not initialized");
	return s_GraphicsSystem.get();
}

GraphicsSystem::~GraphicsSystem()
{
	ASSERT(m_D3DDevice == nullptr && m_immediateContext == nullptr, "GraphicsSystem: must be terminated");
}

void MoonEngine::Graphics::GraphicsSystem::Initialize(HWND window, bool fullscreen)
{
	RECT clientRect{};
	GetClientRect(window, &clientRect);
	UINT width = (UINT)(clientRect.right - clientRect.left);
	UINT height = (UINT)(clientRect.bottom - clientRect.top);

	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = !fullscreen;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	const D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1;


	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		&featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_swapChain,
		&m_D3DDevice,
		nullptr,
		&m_immediateContext);

	ASSERT(SUCCEEDED(hr), "GraphicsSystem: failed to initialize device or swap chain");
	m_swapChain->GetDesc(&m_swapChainDesc);

	Resize(GetBackBufferWidth(), GetBackBufferHeight());

	s_windowMessageHandler.Hook(window, GraphicsSystem::GraphicsSystemMessageHandler);
}

void MoonEngine::Graphics::GraphicsSystem::Terminate()
{
	s_windowMessageHandler.Unhook();

	SafeRelease(m_depthStencilBuffer);
	SafeRelease(m_depthStencilView);
	SafeRelease(m_renderTargetView);
	SafeRelease(m_swapChain);
	SafeRelease(m_immediateContext);
	SafeRelease(m_D3DDevice);
}

void MoonEngine::Graphics::GraphicsSystem::BeginRender()
{
	m_immediateContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
	m_immediateContext->ClearRenderTargetView(m_renderTargetView, (FLOAT*)(&m_clearColor));
	m_immediateContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
}

void MoonEngine::Graphics::GraphicsSystem::EndRender()
{
	m_swapChain->Present(m_vSync, 0);
}

void MoonEngine::Graphics::GraphicsSystem::ToggleFullScreen()
{
	BOOL fullscreen;
	m_swapChain->GetFullscreenState(&fullscreen, nullptr);
	m_swapChain->SetFullscreenState(!fullscreen, nullptr);
}

void MoonEngine::Graphics::GraphicsSystem::Resize(uint32_t width, uint32_t height)
{
	m_immediateContext->OMSetRenderTargets(0, nullptr, nullptr);

	SafeRelease(m_renderTargetView);
	SafeRelease(m_depthStencilView);
	SafeRelease(m_depthStencilBuffer);

	HRESULT hr;
	if (width != GetBackBufferWidth() || height != GetBackBufferHeight())
	{
		hr = m_swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		ASSERT(SUCCEEDED(hr), "GraphicsSystem: failed to access swap chain view");

		m_swapChain->GetDesc(&m_swapChainDesc);
	}

	ID3D11Texture2D* backBuffer = nullptr;
	hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	ASSERT(SUCCEEDED(hr), "GraphicsSystem: failed to get back buffer");

	hr = m_D3DDevice->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);
	ASSERT(SUCCEEDED(hr), "GraphicsSystem: failed to create render target");
	SafeRelease(backBuffer);

	D3D11_TEXTURE2D_DESC depthDesc{};
	depthDesc.Width = GetBackBufferWidth();
	depthDesc.Height = GetBackBufferHeight();
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;

	hr = m_D3DDevice->CreateTexture2D(&depthDesc, nullptr, &m_depthStencilBuffer);
	ASSERT(SUCCEEDED(hr), "GraphicsSystem: faied to create stencil buffer");

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = depthDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	hr = m_D3DDevice->CreateDepthStencilView(m_depthStencilBuffer, &dsvDesc, &m_depthStencilView);
	ASSERT(SUCCEEDED(hr), "GraphicsSystem: failed to create depth stencil view");

	ResetRenderTarget();

	m_viewport.Width = static_cast<float>(GetBackBufferWidth());
	m_viewport.Height = static_cast<float>(GetBackBufferHeight());

	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;
	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	ResetViewport();

}

void MoonEngine::Graphics::GraphicsSystem::ResetRenderTarget()
{
	m_immediateContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
}

void MoonEngine::Graphics::GraphicsSystem::ResetViewport()
{
	m_immediateContext->RSSetViewports(1, &m_viewport);
}

void MoonEngine::Graphics::GraphicsSystem::SetClearColor(const Color& color)
{
	m_clearColor = color;
}

void MoonEngine::Graphics::GraphicsSystem::SetVSync(bool vSync)
{
	m_vSync = vSync ? 1 : 0;
}

uint32_t MoonEngine::Graphics::GraphicsSystem::GetBackBufferWidth() const
{
	return m_swapChainDesc.BufferDesc.Width;
}

uint32_t MoonEngine::Graphics::GraphicsSystem::GetBackBufferHeight() const
{
	return m_swapChainDesc.BufferDesc.Height;
}

float MoonEngine::Graphics::GraphicsSystem::GetBackBufferAspectRatio() const
{
	return static_cast<float>(GetBackBufferWidth()) / static_cast<float>(GetBackBufferHeight());
}

ID3D11Device* MoonEngine::Graphics::GraphicsSystem::GetDevice()
{
	return m_D3DDevice;
}

ID3D11DeviceContext* MoonEngine::Graphics::GraphicsSystem::GetContext()
{
	return m_immediateContext;
}

