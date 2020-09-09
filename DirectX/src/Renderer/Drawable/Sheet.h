#pragma once

#include "Drawable.h"

class Sheet : public DrawableBase<Sheet>
{
public:
	Sheet(Graphics* gfx);
	void Update(float dt);

	virtual DirectX::XMMATRIX GetModelMatrix() const override;
};