#include "Precompiled.h"
#include "Sampler.h"
#include "GraphicsSystem.h"

using namespace MoonEngine;
using namespace MoonEngine::Graphics;

namespace
{
	D3D11_FILTER GetFilter(Sampler::Filter filter)
	{
		switch (filter)
		{
		case Sampler::Filter::Point:
			return D3D11_FILTER_MIN_MAG_MIP_POINT;
			break;
		case Sampler::Filter::Linear:
			return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			break;
		case Sampler::Filter::Anisotropic:
			return D3D11_FILTER_ANISOTROPIC;
			break;
		default:
			ASSERT(false, "Sampler: unhandled filter type");
			break;
		}
		return D3D11_FILTER_MIN_MAG_MIP_POINT;
	}
	D3D11_TEXTURE_ADDRESS_MODE GetAddressMode(Sampler::AddressMode addressMode)
	{
		switch (addressMode)
		{
		case MoonEngine::Graphics::Sampler::AddressMode::Border: 
			return D3D11_TEXTURE_ADDRESS_BORDER;
			break;
		case MoonEngine::Graphics::Sampler::AddressMode::Clamp:
			return D3D11_TEXTURE_ADDRESS_CLAMP;
			break;
		case MoonEngine::Graphics::Sampler::AddressMode::Mirror:
			return D3D11_TEXTURE_ADDRESS_MIRROR;
			break;
		case MoonEngine::Graphics::Sampler::AddressMode::Wrap:
			return D3D11_TEXTURE_ADDRESS_WRAP;
			break;
		default:
			ASSERT(false, "Sampler: unhandled address mode");
			break;
		}
		return D3D11_TEXTURE_ADDRESS_BORDER;
	}
}

Sampler::~Sampler()
{
	ASSERT(m_sampler == nullptr, "Sampler: terminate must be called");
}

void Sampler::Initialize(Filter filter, AddressMode addressMode)
{
	auto d3dFilter = GetFilter(filter);
	auto d3dAddressMode = GetAddressMode(addressMode);

	D3D11_SAMPLER_DESC desc{};
	desc.Filter = d3dFilter;
	desc.AddressU = d3dAddressMode;
	desc.AddressV = d3dAddressMode;
	desc.AddressW = d3dAddressMode;
	desc.BorderColor[0] = 1.0f;
	desc.BorderColor[1] = 1.0f;
	desc.BorderColor[2] = 1.0f;
	desc.BorderColor[3] = 1.0f;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;

	auto device = GraphicsSystem::Get()->GetDevice();
	HRESULT hr = device->CreateSamplerState(&desc, &m_sampler);
	ASSERT(SUCCEEDED(hr), "Sampler: failed to create sampler state");

}

void Sampler::Terminate()
{
	SafeRelease(m_sampler);
}

void Sampler::BindVS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->VSSetSamplers(slot, 1, &m_sampler);
}

void Sampler::BindPS(uint32_t slot) const
{
	auto context = GraphicsSystem::Get()->GetContext();
	context->PSSetSamplers(slot, 1, &m_sampler);
}
