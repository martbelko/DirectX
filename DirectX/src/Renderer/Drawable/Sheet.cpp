#include "pch.h"
#include "Sheet.h"

#include "Renderer/Bindable/Texture.h"
#include "Renderer/Bindable/Sampler.h"
#include "Core/Surface.h"

#include <DirectXMath.h>

namespace dx = DirectX;

Sheet::Sheet(Graphics* gfx)
{
	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			float x, y, z;
			float u, v;
		};

		const std::vector<Vertex> vertices =
		{
			{ -1.0f,  1.0f, 4.0f, 0.0f, 0.0f },
			{  1.0f,  1.0f, 4.0f, 1.0f, 0.0f },
			{  1.0f, -1.0f, 4.0f, 1.0f, 1.0f },
			{ -1.0f, -1.0f, 4.0f, 0.0f, 1.0f }
		};

		static const std::vector<unsigned short> indices =
		{
			0, 1, 2,
			0, 2, 3
		};

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

		auto vs = std::make_unique<VertexShader>(gfx, L"TextureVS.cso");
		auto vsblob = vs->GetBytecode();
		auto ps = std::make_unique<PixelShader>(gfx, L"TexturePS.cso");
		AddStaticBind(std::move(vs));
		AddStaticBind(std::move(ps));

		AddStaticBind(std::make_unique<Texture>(gfx, Surface(L"D:\\dev\\C++\\DirectX\\DirectX\\electro.jpg")));
		AddStaticBind(std::make_unique<Sampler>(gfx));

		static const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, vsblob));
		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}

	AddBind(std::make_unique<TransformConstantBuffer>(gfx, this));
}

void Sheet::Update(float dt)
{

}

DirectX::XMMATRIX Sheet::GetModelMatrix() const
{
	return dx::XMMatrixIdentity();
}
