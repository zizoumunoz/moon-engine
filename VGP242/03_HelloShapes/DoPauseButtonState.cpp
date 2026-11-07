#include "DoPauseButtonState.h"

using namespace MoonEngine;
using namespace MoonEngine::Graphics;
using namespace MoonEngine::Input;
void DoPauseButtonState::Initialize()
{
	// create play triangle
	
	m_vertices.push_back({ { -0.5f, 0.5f, 0.0f }, Colors::White });
	m_vertices.push_back({ { 0.0f,  0.0f, 0.0f }, Colors::White });
	m_vertices.push_back({ { -0.5f, -0.5f, 0.0f }, Colors::White });

	// left pause bar
	m_vertices.push_back({ { 0, 0.5f, 0.0f }, Colors::White });
	m_vertices.push_back({ { 0.25f,  0.5f, 0.0f }, Colors::White });
	m_vertices.push_back({ { 0.0f, -0.5f, 0.0f }, Colors::White });

	m_vertices.push_back({ { 0.25f,  0.5f, 0.0f }, Colors::White });
	m_vertices.push_back({ { 0.25f,  -0.5f, 0.0f }, Colors::White });
	m_vertices.push_back({ { 0.0f, -0.5f, 0.0f }, Colors::White });

	// right pause bar
	m_vertices.push_back({ { 0.4, 0.5f, 0.0f }, Colors::White });
	m_vertices.push_back({ { 0.65f,  0.5f, 0.0f }, Colors::White });
	m_vertices.push_back({ { 0.4f, -0.5f, 0.0f }, Colors::White });

	m_vertices.push_back({ { 0.65f,  0.5f, 0.0f }, Colors::White });
	m_vertices.push_back({ { 0.65f,  -0.5f, 0.0f }, Colors::White });
	m_vertices.push_back({ { 0.4f, -0.5f, 0.0f }, Colors::White });


	// start creating the components that communicate with  GPU
	auto device = GraphicsSystem::Get()->GetDevice();

	// build the vertex buffer, which stores the vertices to give to GPU
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = static_cast<UINT>(m_vertices.size()) * sizeof(Vertex);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = m_vertices.data();

	HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &m_vertexBuffer);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer");
	// ===========

	// bind to the shader file
	std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoColor.fx";

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3DCompileFromFile(
		shaderFilePath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"VS", "vs_5_0",
		shaderFlags, 0,
		&shaderBlob,
		&errorBlob
	);
	if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
	{
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	}
	ASSERT(SUCCEEDED(hr), "Failed to compile vertex shader");

	hr = device->CreateVertexShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&m_vertexShader);
	ASSERT(SUCCEEDED(hr), "Failed to create vertex shader");
	//==========
	// what is the make up of the vertex
	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
	vertexLayout.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT });
	vertexLayout.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT });

	hr = device->CreateInputLayout(
		vertexLayout.data(),
		static_cast<UINT>(vertexLayout.size()),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&m_inputLayout
	);
	ASSERT(SUCCEEDED(hr), "Failed to create input layout");
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
	//====== 
	// bind pixel shader to shader file
	hr = D3DCompileFromFile(
		shaderFilePath.c_str(),
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"PS", "ps_5_0",
		shaderFlags, 0,
		&shaderBlob,
		&errorBlob);
	if (errorBlob != nullptr && errorBlob->GetBufferPointer() != nullptr)
	{
		LOG("%s", static_cast<const char*>(errorBlob->GetBufferPointer()));
	}
	ASSERT(SUCCEEDED(hr), "Failed to compile pixel shader");

	hr = device->CreatePixelShader(
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&m_pixelShader);
	ASSERT(SUCCEEDED(hr), "Failed to create pixel shader");
	SafeRelease(shaderBlob);
	SafeRelease(errorBlob);
}

void DoPauseButtonState::Update(float deltaTime)
{
	// leave for now
	if (InputSystem::Get()->IsKeyPressed(KeyCode::SPACE))
	{
		MainApp().ChangeState("DoSomethingState");
	}
}

void DoPauseButtonState::Render()
{
	auto context = GraphicsSystem::Get()->GetContext();

	// bind all the buffers
	context->VSSetShader(m_vertexShader, nullptr, 0);
	context->IASetInputLayout(m_inputLayout);
	context->PSSetShader(m_pixelShader, nullptr, 0);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// draw
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	context->Draw(static_cast<UINT>(m_vertices.size()), 0);
}

void DoPauseButtonState::Terminate()
{
	m_vertices.clear();
	SafeRelease(m_pixelShader);
	SafeRelease(m_inputLayout);
	SafeRelease(m_vertexShader);
	SafeRelease(m_vertexBuffer);
}