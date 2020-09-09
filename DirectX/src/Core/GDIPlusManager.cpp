#include "pch.h"
#include "GDIPlusManager.h"

#include <algorithm>
#include <Windows.h>
#include <gdiplus.h>

#pragma comment(lib, "gdiplus.lib")

ULONG_PTR GDIPlusManager::s_Token;
int GDIPlusManager::s_RefCount;

GDIPlusManager::GDIPlusManager()
{
	if (s_RefCount++ == 0)
	{
		Gdiplus::GdiplusStartupInput input;
		input.GdiplusVersion = 1;
		input.DebugEventCallback = nullptr;
		input.SuppressBackgroundThread = false;
		Gdiplus::GdiplusStartup(&s_Token, &input, nullptr);
	}
}

GDIPlusManager::~GDIPlusManager()
{
	if (--s_RefCount == 0)
		Gdiplus::GdiplusShutdown(s_Token);
}
