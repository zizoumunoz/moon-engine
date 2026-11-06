#include "Precompiled.h"
#include "PixelShader.h"
#include "GraphicsSystem.h"

using namespace MoonEngine;
using namespace MoonEngine::Graphics;

void MoonEngine::Graphics::PixelShader::Initialize(const std::filesystem::path& shaderPath)
{
	auto device = GraphicsSystem::Get()->GetDevice();

	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT hr = D3DCompileFromFile(
		shaderPath.c_str(),
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

void MoonEngine::Graphics::PixelShader::Terminate()
{
	SafeRelease(m_pixelShader);
}

void MoonEngine::Graphics::PixelShader::Bind()
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->PSSetShader(m_pixelShader, nullptr, 0);
}
