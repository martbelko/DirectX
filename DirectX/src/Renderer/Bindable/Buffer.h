#pragma once

#include "Bindable.h"

#include <vector>

class VertexBuffer : public Bindable
{
public:
	template<typename T>
	VertexBuffer(Graphics* gfx, const std::vector<T>& vertices)
		: Bindable(gfx), m_Stride(sizeof(T))
	{
		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.ByteWidth = sizeof(T) * vertices.size();
		bd.StructureByteStride = sizeof(T);
		D3D11_SUBRESOURCE_DATA sd;
		sd.pSysMem = vertices.data();

		Device()->CreateBuffer(&bd, &sd, &m_Buffer);
	}

	virtual ~VertexBuffer() override;

	virtual void Bind() override;
protected:
	unsigned int m_Stride;
	ID3D11Buffer* m_Buffer;
};

class IndexBuffer : public Bindable
{
public:
	IndexBuffer(Graphics* gfx, const std::vector<unsigned short>& indices);
	virtual ~IndexBuffer() override;

	virtual void Bind() override;

	unsigned int GetCount() const;
protected:
	unsigned int m_Count;
	ID3D11Buffer* m_Buffer;
};

class ConstantBuffer : public Bindable
{
public:
	ConstantBuffer(Graphics* gfx, const void* consts, size_t size);
	ConstantBuffer(Graphics* gfx, size_t size);
	virtual ~ConstantBuffer() override;

	void Update(const void* consts, size_t size);
protected:
	ID3D11Buffer* m_Buffer;
};

class VertexConstantBuffer : public ConstantBuffer
{
public:
	VertexConstantBuffer(Graphics* gfx, const void* consts, size_t size);
	VertexConstantBuffer(Graphics* gfx, size_t size);

	virtual void Bind() override;
};

class PixelConstantBuffer : public ConstantBuffer
{
public:
	PixelConstantBuffer(Graphics* gfx, const void* consts, size_t size);
	PixelConstantBuffer(Graphics* gfx, size_t size);

	virtual void Bind() override;
};

class Drawable;

class TransformConstantBuffer : public ConstantBuffer
{
public:
	TransformConstantBuffer(Graphics* gfx, Drawable* parent);

	virtual void Bind() override;
protected:
	Drawable* m_Parent;
};
