#include "pch.h"
#include "Drawable.h"

void Drawable::AddBind(std::unique_ptr<Bindable> bind)
{
	m_Bindables.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> pIndexBuffer)
{
	m_IndexBuffer = std::move(pIndexBuffer);
}

void Drawable::Draw(Graphics* gfx)
{
	for (auto& bind : GetStaticBinds())
		bind->Bind();

	for (auto& bind : m_Bindables)
	{
		bind->Bind();
	}

	const std::unique_ptr<IndexBuffer>& indexBuffer = m_IndexBuffer ? m_IndexBuffer : GetStaticIndexBuffer();
	indexBuffer->Bind();
	gfx->DrawIndexed(indexBuffer->GetCount());
}
