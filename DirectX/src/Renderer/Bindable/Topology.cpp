#include "pch.h"
#include "Topology.h"

Topology::Topology(Graphics* gfx, D3D11_PRIMITIVE_TOPOLOGY type)
	: Bindable(gfx), m_Type(type)
{
}

void Topology::Bind()
{
	Context()->IASetPrimitiveTopology(m_Type);
}
