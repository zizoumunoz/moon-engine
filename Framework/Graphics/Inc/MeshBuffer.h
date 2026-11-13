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
		template <class MeshType>
		void Initialize(const MeshType& mesh)
		{
			Initialize(
				mesh.vertices.data(),
				static_cast<uint32_t>(sizeof(MeshType::VertexType)),
				static_cast<uint32_t>(mesh.vertices.size()),
				mesh.indices.data(),
				static_cast<uint32_t>(mesh.indices.size()));
		}
		void Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount);
		void Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, const void* indices, uint32_t indexCount);
		void Terminate();

		void SetTopology(Topology topology);
		void Render() const;
	private:
		void CreateVertexBuffer(const void* vertices, uint32_t vertexSize, uint32_t vertexCount);
		void CreateIndexBuffer(const void* indices, uint32_t indexCount);

		ID3D11Buffer* m_vertexBuffer = nullptr;
		ID3D11Buffer* m_indexBuffer = nullptr;

		D3D11_PRIMITIVE_TOPOLOGY m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		uint32_t m_vertexSize;
		uint32_t m_vertexCount;
		uint32_t m_indexCount;
	};
}