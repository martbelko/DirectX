#pragma once

#include "Bindable.h"

class Topology : public Bindable
{
public:
	Topology(Graphics* gfx, D3D11_PRIMITIVE_TOPOLOGY type);
	virtual ~Topology() override = default;

	virtual void Bind() override;
protected:
	D3D11_PRIMITIVE_TOPOLOGY m_Type;
};