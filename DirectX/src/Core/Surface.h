#pragma once

#include <string_view>

#include "Core/Colors.h"

class Surface
{
public:
	Surface(const std::wstring_view& filepath);
	Surface(int width, int height);
	Surface(const Surface& surface);
	Surface& operator=(const Surface& surface);
	~Surface();

	void PutPixel(int x, int y, Color color);
	Color GetPixel(int x, int y) const;

	int GetWidth() const;
	int GetHeight() const;
	const void* GetBufferPtr() const;
private:
	Color* m_Pixels = nullptr;
	int m_Width, m_Height;
};