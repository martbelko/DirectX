#pragma once

#include "Renderer/Bindable/Buffer.h"
#include "Renderer/Bindable/Shader.h"
#include "Renderer/Bindable/InputLayout.h"
#include "Renderer/Bindable/Topology.h"

#include <memory>

class IndexBuffer;

class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual ~Drawable() = default;

	void AddBind(std::unique_ptr<Bindable> pBindable);
	void AddIndexBuffer(std::unique_ptr<IndexBuffer> pIndexBuffer);

	void Draw(Graphics* gfx);

	virtual DirectX::XMMATRIX GetModelMatrix() const = 0;
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const = 0;
	virtual const std::unique_ptr<IndexBuffer>& GetStaticIndexBuffer() const = 0;
private:
	std::vector<std::unique_ptr<Bindable>> m_Bindables;
	std::unique_ptr<IndexBuffer> m_IndexBuffer = nullptr;
};

template<typename T>
class DrawableBase : public Drawable
{
protected:
	bool IsStaticInitialized() const
	{
		return !s_Binds.empty();
	}

	void AddStaticBind(std::unique_ptr<Bindable> pBindable)
	{
		s_Binds.push_back(std::move(pBindable));
	}

	void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> pIndexBuffer)
	{
		s_IndexBuffer = std::move(pIndexBuffer);
	}

	const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const override final
	{
		return s_Binds;
	}

	const std::unique_ptr<IndexBuffer>& GetStaticIndexBuffer() const override final
	{
		return s_IndexBuffer;
	}
private:
	static std::vector<std::unique_ptr<Bindable>> s_Binds;
	static std::unique_ptr<IndexBuffer> s_IndexBuffer;
};

template<typename T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::s_Binds;

template<typename T>
std::unique_ptr<IndexBuffer> DrawableBase<T>::s_IndexBuffer = nullptr;
