#pragma once

namespace MoonEngine::Graphics
{
	class Texture
	{
	public:
		// removes reference of texture in the gpu
		static void UnbindPS(uint32_t slot);

		Texture() = default;
		virtual ~Texture();

		// delete copy constructors
		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		// allow with noexcept
		Texture(Texture&& rhs) noexcept;
		Texture& operator=(Texture&& rhs);

		virtual void Initialize(const std::filesystem::path& fileName);
		virtual void Terminate();

		void BindVS(uint32_t slot) const;
		void BindPS(uint32_t slot) const;

		void* GetRawData() const;

	protected:
		ID3D11ShaderResourceView* m_shaderResourceView = nullptr;
	};
}
