#include "Precompiled.h"
#include "MeshBuffer.h"
#include "GraphicsSystem.h"

using namespace MoonEngine;
using namespace MoonEngine::Graphics;


void MeshBuffer::Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount)
{
	CreateVertexBuffer(vertices, vertexSize, vertexCount);
}

void MeshBuffer::Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount, const void* indices, uint32_t indexCount)
{
	CreateVertexBuffer(vertices, vertexSize, vertexCount);
	CreateIndexBuffer(indices, indexCount);

}

void MoonEngine::Graphics::MeshBuffer::Terminate()
{
	SafeRelease(m_vertexBuffer);
	SafeRelease(m_indexBuffer);
}

void MoonEngine::Graphics::MeshBuffer::SetTopology(Topology topology)
{
	switch (topology)
	{
	case MoonEngine::Graphics::MeshBuffer::Topology::Points: m_topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST; break;
	case MoonEngine::Graphics::MeshBuffer::Topology::Lines:m_topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST; break;
	case MoonEngine::Graphics::MeshBuffer::Topology::Triangles:m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;
	default:
		break;
	}
}

void MoonEngine::Graphics::MeshBuffer::Render() const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->IASetPrimitiveTopology(m_topology);

	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &m_vertexSize, &offset);
	if (m_indexBuffer != nullptr)
	{
		context->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		context->DrawIndexed((UINT)m_indexCount, 0, 0);
	}
	else
	{
		context->Draw(m_vertexCount, 0);
	}
}

void MoonEngine::Graphics::MeshBuffer::CreateVertexBuffer(const void* vertices, uint32_t vertexSize, uint32_t vertexCount)
{
	m_vertexSize = vertexSize;
	m_vertexCount = vertexCount;
	auto device = GraphicsSystem::Get()->GetDevice();

	// build the vertex buffer, which stores the vertices to give to GPU
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = vertexCount * vertexSize;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;

	HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &m_vertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer");
}

void MeshBuffer::CreateIndexBuffer(const void* indices, uint32_t indexCount)
{
	if (indexCount == 0)
	{
		return;

	}

	m_indexCount = indexCount;

	auto device = GraphicsSystem::Get()->GetDevice();

	// index buffer
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = static_cast<UINT>(indexCount) * sizeof(uint32_t);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = indices;

	HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &m_indexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create index buffer");
}
