#include "pch.h"
#include "Texture.h"

#include "Core/Surface.h"

Texture::Texture(Graphics* gfx, const Surface& s)
	: Bindable(gfx)
{
	D3D11_TEXTURE2D_DESC textureDesc = { };
	textureDesc.Width = s.GetWidth();
	textureDesc.Height = s.GetHeight();
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA sd = { };
	sd.pSysMem = s.GetBufferPtr();
	sd.SysMemPitch = s.GetWidth() * sizeof(Color);
	ID3D11Texture2D* texture;
	Device()->CreateTexture2D(&textureDesc, &sd, &texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = { };
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	Device()->CreateShaderResourceView(texture, &srvDesc, &m_TextureView);

	texture->Release();
}

Texture::~Texture()
{
	m_TextureView->Release();
}

void Texture::Bind()
{
	Context()->PSSetShaderResources(0, 1, &m_TextureView);
}
