#include "pch.h"
#include "Buffer.h"

#include <DirectXMath.h>

#include "Renderer/Drawable/Drawable.h"

namespace dx = DirectX;

VertexBuffer::~VertexBuffer()
{
	if (m_Buffer)
		m_Buffer->Release();
}

void VertexBuffer::Bind()
{
	const UINT offset = 0;
	Context()->IASetVertexBuffers(0, 1, &m_Buffer, &m_Stride, &offset);
}

IndexBuffer::IndexBuffer(Graphics* gfx, const std::vector<unsigned short>& indices)
	: Bindable(gfx), m_Count(indices.size())
{
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.ByteWidth = sizeof(unsigned short) * indices.size();
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices.data();

	Device()->CreateBuffer(&ibd, &isd, &m_Buffer);
}

IndexBuffer::~IndexBuffer()
{
	if (m_Buffer)
		m_Buffer->Release();
}

void IndexBuffer::Bind()
{
	Context()->IASetIndexBuffer(m_Buffer, DXGI_FORMAT_R16_UINT, 0);
}

unsigned int IndexBuffer::GetCount() const
{
	return m_Count;
}

ConstantBuffer::ConstantBuffer(Graphics* gfx, const void* consts, size_t size)
	: Bindable(gfx)
{
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0;
	cbd.ByteWidth = size;
	cbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA csd;
	csd.pSysMem = consts;

	Device()->CreateBuffer(&cbd, &csd, &m_Buffer);
}

ConstantBuffer::ConstantBuffer(Graphics* gfx, size_t size)
	: Bindable(gfx)
{
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0;
	cbd.ByteWidth = size;
	cbd.StructureByteStride = 0;

	Device()->CreateBuffer(&cbd, nullptr, &m_Buffer);
}

ConstantBuffer::~ConstantBuffer()
{
	if (m_Buffer)
		m_Buffer->Release();
}

void ConstantBuffer::Update(const void* consts, size_t size)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	Context()->Map(m_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, consts, size);
	Context()->Unmap(m_Buffer, 0);
}

VertexConstantBuffer::VertexConstantBuffer(Graphics* gfx, const void* consts, size_t size)
	: ConstantBuffer(gfx, consts, size)
{
}

VertexConstantBuffer::VertexConstantBuffer(Graphics* gfx, size_t size)
	: ConstantBuffer(gfx, size)
{
}

void VertexConstantBuffer::Bind()
{
	Context()->VSSetConstantBuffers(0, 1, &(this->m_Buffer));
}

PixelConstantBuffer::PixelConstantBuffer(Graphics* gfx, const void* consts, size_t size)
	: ConstantBuffer(gfx, consts, size)
{
}

PixelConstantBuffer::PixelConstantBuffer(Graphics* gfx, size_t size)
	: ConstantBuffer(gfx, size)
{
}

void PixelConstantBuffer::Bind()
{
	Context()->PSSetConstantBuffers(0, 1, &m_Buffer);
}

TransformConstantBuffer::TransformConstantBuffer(Graphics* gfx, Drawable* parent)
	: ConstantBuffer(gfx, sizeof(dx::XMMATRIX)), m_Parent(parent)
{
}

void TransformConstantBuffer::Bind()
{
	dx::XMMATRIX modelMatrix = dx::XMMatrixTranspose(
		m_Parent->GetModelMatrix() *
		GetProjectionMatrix());

	Update(&modelMatrix, sizeof(modelMatrix));
	Context()->VSSetConstantBuffers(0, 1, &m_Buffer);
}
