#pragma once

#include "Bindable.h"

class Surface;

class Texture : public Bindable
{
public:
	Texture(Graphics* gfx, const Surface& surface);
	virtual ~Texture() override;
	virtual void Bind() override;
protected:
	ID3D11ShaderResourceView* m_TextureView;
};