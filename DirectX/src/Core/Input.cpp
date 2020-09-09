#include "pch.h"
#include "Input.h"

#include "Core/Core.h"

#include "Core/Win.h"

bool Input::IsKeyDown(unsigned char keycode)
{
	return GetAsyncKeyState(keycode) & BIT(31);
}

std::pair<float, float> Input::GetMousePosition()
{
	POINT mp;
	GetCursorPos(&mp);
	return { mp.x, mp.y };
}

float Input::GetMouseX()
{
	POINT mp;
	GetCursorPos(&mp);
	return mp.x;
}

float Input::GetMouseY()
{
	POINT mp;
	GetCursorPos(&mp);
	return mp.y;
}
