#pragma once

#include "Renderer/Graphics.h"

class Bindable
{
public:
	Bindable(Graphics* gfx);
	virtual ~Bindable() = default;

	virtual void Bind() = 0;
protected:
	ID3D11Device* Device();
	ID3D11DeviceContext* Context();
	DirectX::XMMATRIX GetProjectionMatrix() const;
private:
	Graphics* m_Gfx;
};