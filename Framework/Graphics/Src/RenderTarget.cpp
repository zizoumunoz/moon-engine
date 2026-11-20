#include "Precompiled.h"
#include "RenderTarget.h"

#include "GraphicsSystem.h"

using namespace MoonEngine;
using namespace MoonEngine::Graphics;

namespace
{
	DXGI_FORMAT GetDXGIFormat(RenderTarget::Format format)
	{
		switch (format)
		{
		case MoonEngine::Graphics::RenderTarget::Format::RGBA_U8:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		case MoonEngine::Graphics::RenderTarget::Format::RGBA_U32:
			return DXGI_FORMAT_R32G32B32A32_UINT;
			break;
		default:
			ASSERT(false, "RenderTarget: unahndle format");
			break;

		}
		return DXGI_FORMAT_R8G8B8A8_UNORM;
	}
}

MoonEngine::Graphics::RenderTarget::~RenderTarget()
{
	ASSERT(m_renderTargetView == nullptr && m_depthStencilView == nullptr, "RenderTarget: Terminate must be called");
}

void MoonEngine::Graphics::RenderTarget::Initialize(const std::filesystem::path& filename)
{
	// overridden from Texture, dont use
	ASSERT(false, "RenderTarget: file loading not supported");
}

void MoonEngine::Graphics::RenderTarget::Initialize(uint32_t width, uint32_t height, Format format)
{
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = GetDXGIFormat(format);
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	auto device = GraphicsSystem::Get()->GetDevice();
	ID3D11Texture2D* texture = nullptr;
	HRESULT hr = device->CreateTexture2D(&desc, nullptr, &texture);
	ASSERT(SUCCEEDED(hr), "RenderTarget: failed to create texture");

	hr = device->CreateShaderResourceView(texture, nullptr, &m_shaderResourceView);
	ASSERT(SUCCEEDED(hr), "RenderTexture: failed to create shader resource view");

	hr = device->CreateRenderTargetView(texture, nullptr, &m_renderTargetView);
	ASSERT(SUCCEEDED(hr), "RenderTexture: failed to create render view");

	SafeRelease(texture);

	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hr = device->CreateTexture2D(&desc, nullptr, &texture);
	ASSERT(SUCCEEDED(hr), "RenderTarget: failed to create depth stencil texture");

	hr = device->CreateDepthStencilView(texture, nullptr, &m_depthStencilView);
	ASSERT(SUCCEEDED(hr), "RenderTarget: failed to create depth stencil view");

	SafeRelease(texture);

	m_viewport.TopLeftX = 0.0f;
	m_viewport.TopLeftY = 0.0f;
	m_viewport.Width = static_cast<float>(width);
	m_viewport.Height = static_cast<float>(height);
	m_viewport.MinDepth = 0.0f;
	m_viewport.MaxDepth = 1.0f;	

}

void MoonEngine::Graphics::RenderTarget::Terminate()
{
	Texture::Terminate();
	SafeRelease(m_renderTargetView);
	SafeRelease(m_depthStencilView);
}

void MoonEngine::Graphics::RenderTarget::BeginRender(Color clearColor)
{
	auto context = GraphicsSystem::Get()->GetContext();

	// store the current values
	UINT numViewports = 1;
	context->OMGetRenderTargets(1, &m_oldRenderTargetView, &m_oldDepthStencilView);
	context->RSGetViewports(&numViewports, &m_oldViewport);

	// override with render target info
	context->ClearRenderTargetView(m_renderTargetView, &clearColor.r);

}

void MoonEngine::Graphics::RenderTarget::EndRender()
{
}
