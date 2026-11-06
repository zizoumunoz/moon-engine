#pragma once

namespace MoonEngine::Graphics
{
	class VertexShader final
	{
	public:
		template<class VertexType>
		void Initialize(const std::filesystem::path& shaderPath)
		{
			Initialize(shaderPath, VertexType::Format);
		}

		void Initialize(const std::filesystem::path& shaderPath, uint32_t format);
		void Terminate();
		void Bind();
	private:
		ID3D11VertexShader* m_vertexShader = nullptr;
		ID3D11InputLayout* m_inputLayout = nullptr;
	};
}