#include "pch.h"
#include "Box.h"

#include <DirectXMath.h>

namespace dx = DirectX;

Box::Box(Graphics* gfx, float x, float y, float z, float velX /*= 0.0f*/, float velY /*= 0.0f*/, float velZ /*= 0.0f*/)
{
	m_Position = { x, y, z };
	m_Velocity = { velX, velY, velZ };

	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			float x, y, z;
		};

		const std::vector<Vertex> vertices =
		{
			{ -1.0f, -1.0f, -1.0f },
			{  1.0f, -1.0f, -1.0f },
			{ -1.0f,  1.0f, -1.0f },
			{  1.0f,  1.0f, -1.0f },
			{ -1.0f, -1.0f,  1.0f },
			{  1.0f, -1.0f,  1.0f },
			{ -1.0f,  1.0f,  1.0f },
			{  1.0f,  1.0f,  1.0f }
		};

		static const std::vector<unsigned short> indices =
		{
			0, 2, 1, 2, 3, 1,
			1, 3, 5, 3, 7, 5,
			2, 6, 3, 3, 6, 7,
			4, 5, 7, 4, 7, 6,
			0, 4, 2, 2, 4, 6,
			0, 1, 4, 1, 5, 4
		};

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, vertices));
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

		auto vs = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
		auto vsblob = vs->GetBytecode();
		auto ps = std::make_unique<PixelShader>(gfx, L"PixelShader.cso");
		AddStaticBind(std::move(vs));
		AddStaticBind(std::move(ps));

		static const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, vsblob));
		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		struct ConstantBuffer2
		{
			struct
			{
				float r, g, b, a;
			} faceColors[6];
		};

		const ConstantBuffer2 cb2 =
		{
			{
				{ 1.0f, 0.0f, 1.0f },
				{ 1.0f, 0.0f, 0.0f },
				{ 0.0f, 1.0f, 0.0f },
				{ 0.0f, 0.0f, 1.0f },
				{ 1.0f, 1.0f, 0.0f },
				{ 0.0f, 1.0f, 1.0f },
			}
		};

		PixelConstantBuffer pcb(gfx, &cb2, sizeof(cb2));
		AddStaticBind(std::make_unique<PixelConstantBuffer>(gfx, &cb2, sizeof(cb2)));
	}

	AddBind(std::make_unique<TransformConstantBuffer>(gfx, this));
}

void Box::Update(float dt)
{
	m_Position.x += m_Velocity.x * dt;
	m_Position.y += m_Velocity.y * dt;
	m_Position.z += m_Velocity.z * dt;
}

BoxPosition& Box::Position()
{
	return m_Position;
}

BoxVelocity& Box::Velocity()
{
	return m_Velocity;
}

dx::XMMATRIX Box::GetModelMatrix() const
{
	return dx::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
}
