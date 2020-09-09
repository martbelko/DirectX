#pragma once

class Input
{
public:
	static bool IsKeyDown(unsigned char keycode);
	static std::pair<float, float> GetMousePosition();
	static float GetMouseX();
	static float GetMouseY();
};
