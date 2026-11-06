#include "Precompiled.h"
#include "MeshBuffer.h"
#include "GraphicsSystem.h"

using namespace MoonEngine;
using namespace MoonEngine::Graphics;


void MoonEngine::Graphics::MeshBuffer::Initialize(const void* vertices, uint32_t vertexSize, uint32_t vertexCount)
{
	CreateVertexBuffer(vertices, vertexSize, vertexCount);
}

void MoonEngine::Graphics::MeshBuffer::Terminate()
{
	SafeRelease(m_vertexBuffer);
}

void MoonEngine::Graphics::MeshBuffer::SetTopology(Topology topology)
{
	switch (topology)
	{
	case MoonEngine::Graphics::MeshBuffer::Topology::Points:
		m_topology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		break;
	case MoonEngine::Graphics::MeshBuffer::Topology::Lines:
		m_topology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		break;
	case MoonEngine::Graphics::MeshBuffer::Topology::Triangles:
		m_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		break;
	default:
		break;
	}
}

void MoonEngine::Graphics::MeshBuffer::Render() const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &m_vertexSize, &offset);
	context->Draw(m_vertexCount, 0);
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
