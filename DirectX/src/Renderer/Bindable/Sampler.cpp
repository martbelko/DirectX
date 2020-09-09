#include "pch.h"
#include "Sampler.h"

Sampler::Sampler(Graphics* gfx)
	: Bindable(gfx)
{
	D3D11_SAMPLER_DESC samplerDesc = { };
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	Device()->CreateSamplerState(&samplerDesc, &m_Sampler);
}

Sampler::~Sampler()
{
	m_Sampler->Release();
}

void Sampler::Bind()
{
	Context()->PSSetSamplers(0, 1, &m_Sampler);
}
