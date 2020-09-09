#include "pch.h"
#include "Surface.h"

#include <Windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

namespace gdi = Gdiplus;

Surface::Surface(const std::wstring_view& filepath)
{
	gdi::Bitmap bitmap(filepath.data());
	if (bitmap.GetLastStatus() != gdi::Ok)
	{
		MessageBoxA(0, "Gdiplus failed", "Gdiplus Error", 0);
		__debugbreak();
	}
	else
	{
		m_Width = bitmap.GetWidth();
		m_Height = bitmap.GetHeight();
		m_Pixels = new Color[m_Width * m_Height];
		const bool isAlpha = gdi::IsAlphaPixelFormat(bitmap.GetPixelFormat());
		for (int y = 0; y < m_Height; ++y)
		{
			for (int x = 0; x < m_Width; ++x)
			{
				gdi::Color pixel;
				bitmap.GetPixel(x, y, &pixel);
				if (isAlpha)
					PutPixel(x, y, { pixel.GetA(), pixel.GetR(), pixel.GetG(), pixel.GetB() });
				else
					PutPixel(x, y, { pixel.GetR(), pixel.GetG(), pixel.GetB() });
			}
		}
	}
}

Surface::Surface(int width, int height)
	: m_Width(width), m_Height(height), m_Pixels(new Color[m_Width * m_Height])
{
}

Surface::Surface(const Surface& surface)
	: Surface(surface.m_Width, surface.m_Height)
{
	memcpy(m_Pixels, surface.m_Pixels, surface.m_Width * surface.m_Height * sizeof(Color));
}


Surface& Surface::operator=(const Surface& surface)
{
	if (this != &surface)
	{
		m_Width = surface.m_Width;
		m_Height = surface.m_Height;
		if (m_Pixels)
			delete[] m_Pixels;

		m_Pixels = new Color[m_Width * m_Height];
		memcpy(m_Pixels, surface.m_Pixels, surface.m_Width * surface.m_Height * sizeof(Color));
	}

	return *this;
}

Surface::~Surface()
{
	if (m_Pixels)
		delete[] m_Pixels;
}

void Surface::PutPixel(int x, int y, Color color)
{
	m_Pixels[y * m_Width + x] = color;
}

Color Surface::GetPixel(int x, int y) const
{
	return m_Pixels[y * m_Width + x];
}

int Surface::GetWidth() const
{
	return m_Width;
}

int Surface::GetHeight() const
{
	return m_Height;
}

const void* Surface::GetBufferPtr() const
{
	return m_Pixels;
}
