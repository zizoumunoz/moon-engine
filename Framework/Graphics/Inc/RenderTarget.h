#pragma once

#include "Color.h"
#include "Texture.h"

namespace MoonEngine::Graphics
{
	class RenderTarget : public Texture
	{
	public:
		enum class Format
		{
			RGBA_U8,
			RGBA_U32,
		};

		RenderTarget() = default;
		~RenderTarget() override;

		void Initialize(const std::filesystem::path& filename) override;
		void Initialize(uint32_t width, uint32_t height, Format format);
		void Terminate() override;

		void BeginRender(Color clearColor = Colors::Black);
		void EndRender();
	private:
		ID3D11RenderTargetView* m_renderTargetView = nullptr;
		ID3D11DepthStencilView* m_depthStencilView = nullptr;
		D3D11_VIEWPORT m_viewport{};

		// referneces to the previous state to reset after drawing to the texture
		ID3D11RenderTargetView* m_oldRenderTargetView = nullptr;
		ID3D11DepthStencilView* m_oldDepthStencilView = nullptr;
		D3D11_VIEWPORT m_oldViewport{};
	};
}
