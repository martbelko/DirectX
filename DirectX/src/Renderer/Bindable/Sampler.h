#pragma once

#include "Bindable.h"

class Sampler : public Bindable
{
public:
	Sampler(Graphics* gfx);
	virtual ~Sampler() override;

	virtual void Bind() override;
protected:
	ID3D11SamplerState* m_Sampler;
};