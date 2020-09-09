#include "pch.h"
#include "Bindable.h"

#include "Renderer/Graphics.h"

Bindable::Bindable(Graphics* gfx)
	: m_Gfx(gfx)
{

}

ID3D11Device* Bindable::Device()
{
	return m_Gfx->pDevice;
}

ID3D11DeviceContext* Bindable::Context()
{
	return m_Gfx->pContext;
}

DirectX::XMMATRIX Bindable::GetProjectionMatrix() const
{
	return m_Gfx->GetProjectionMatrix();
}
