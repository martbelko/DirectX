#pragma once

#include "Win.h"
#include "Renderer/Graphics.h"

#include <string>
#include <optional>

class Window
{
public:
	Window(unsigned int width = 1280, unsigned int height = 720, const std::wstring& title = L"Default Title");
	~Window();

	void SetTitle(const wchar_t* title);

	std::optional<int> Update();
	Graphics* Gfx();

	std::pair<unsigned int, unsigned int> GetWindowSize();
	unsigned int GetWindowWidth();
	unsigned int GetWindowHeight();

	std::pair<unsigned int, unsigned int> GetClientSize();
	unsigned int GetClientWidth();
	unsigned int GetClientHeight();
private:
	static LRESULT CALLBACK WindowMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK StaticWindowMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK WindowMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:
	class WindowClass
	{
	public:
		WindowClass();
		~WindowClass();
		WindowClass(const WindowClass& wndClass) = delete;
		WindowClass& operator=(const WindowClass& wndClass) = delete;

		const wchar_t* GetClassName() const;
		HINSTANCE GetInstance() const;
	private:
		HINSTANCE m_hInstance;
		const wchar_t* m_ClassName = L"DefaultClassName";
	};

	HWND m_hWnd;
	unsigned int m_WindowWidth, m_WindowHeight;
	unsigned int m_ClientWidth, m_ClientHeight;
	std::wstring m_Title;
	static WindowClass m_WndClass;

	Graphics* m_Gfx = nullptr;
};
