#pragma once

#include "Drawable.h"

struct BoxPosition
{
	float x, y, z;
};

struct BoxVelocity
{
	float x, y, z;
};

class Box : public DrawableBase<Box>
{
public:
	Box(Graphics* gfx, float x, float y, float z, float velX = 0.0f, float velY = 0.0f, float velZ = 0.0f);
	void Update(float dt);

	BoxPosition& Position();
	BoxVelocity& Velocity();

	virtual DirectX::XMMATRIX GetModelMatrix() const override;
private:
	BoxPosition m_Position;
	BoxVelocity m_Velocity;
};
