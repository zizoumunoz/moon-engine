#pragma once

namespace MoonEngine::Graphics
{
	class MeshBuffer final
	{
	public:
		enum class Topology
		{
			Points,
			Lines,
			Triangles
		};

		void Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount);
		void Terminate();

		void SetTopology(Topology topology);
		void Render() const;
	private:
		void CreateVertexBuffer(const void* vertices, uint32_t vertexSize, uint32_t vertexCount);
		
		ID3D11Buffer* m_vertexBuffer = nullptr;
		D3D11_PRIMITIVE_TOPOLOGY m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		uint32_t m_vertexSize;
		uint32_t m_vertexCount;
	};
}