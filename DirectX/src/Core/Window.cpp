#include "pch.h"
#include "Window.h"

#include "ImGui/imgui_impl_win32.h"
#include "Win.h"

Window::Window(unsigned int width, unsigned int height, const std::wstring& title)
	: m_ClientWidth(width), m_ClientHeight(height), m_Title(title)
{
	RECT wr;
	wr.left = 100;
	wr.right = wr.left + width;
	wr.top = 100;
	wr.bottom = wr.top + height;
	const DWORD style = WS_MINIMIZEBOX | WS_SYSMENU;
	AdjustWindowRectEx(&wr, style, FALSE, 0);
	m_WindowWidth = wr.right - wr.left;
	m_WindowHeight = wr.bottom - wr.top;
	m_hWnd = CreateWindowEx(0, TEXT("TestClass"), title.c_str(), WS_MINIMIZEBOX | WS_SYSMENU | WS_CAPTION,
		200, 200, m_WindowWidth, m_WindowHeight, 0, 0, m_WndClass.GetInstance(), this);

	ShowWindow(m_hWnd, SW_SHOW);
	ImGui_ImplWin32_Init(m_hWnd);

	m_Gfx = new Graphics(m_hWnd, this);
}

Window::~Window()
{
	if (m_Gfx)
		delete m_Gfx;
	ImGui_ImplWin32_Shutdown();
	DestroyWindow(m_hWnd);
}

LRESULT CALLBACK Window::WindowMessageSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCT* const pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		Window* const pWindow = static_cast<Window*>(pCreate->lpCreateParams);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Window::StaticWindowMessageHandler));

		return pWindow->WindowMessageHandler(hWnd, msg, wParam, lParam);
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::StaticWindowMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	return pWnd->WindowMessageHandler(hWnd, msg, wParam, lParam);
}

LRESULT Window::WindowMessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		case WM_MOVE:
		{

		}
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

Window::WindowClass Window::m_WndClass;

void Window::SetTitle(const wchar_t* title)
{
	SetWindowText(m_hWnd, title);
}

std::optional<int> Window::Update()
{
	MSG msg;
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return msg.wParam;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}

Graphics* Window::Gfx()
{
	return m_Gfx;
}

std::pair<unsigned int, unsigned int> Window::GetWindowSize()
{
	RECT rect;
	GetWindowRect(m_hWnd, &rect);
	return std::make_pair<unsigned int, unsigned int>(rect.right - rect.left, rect.bottom - rect.top);
}

unsigned int Window::GetWindowWidth()
{
	return m_WindowWidth;
}

unsigned int Window::GetWindowHeight()
{
	return m_WindowHeight;
}

std::pair<unsigned int, unsigned int> Window::GetClientSize()
{
	return { m_ClientWidth, m_ClientHeight };
}

unsigned int Window::GetClientWidth()
{
	return m_ClientWidth;
}

unsigned int Window::GetClientHeight()
{
	return m_ClientHeight;
}

Window::WindowClass::WindowClass()
	: m_hInstance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.hInstance = m_hInstance;
	wc.lpszClassName = TEXT("TestClass");
	wc.lpfnWndProc = Window::WindowMessageSetup;
	RegisterClassEx(&wc);
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(GetClassName(), GetInstance());
}

const wchar_t* Window::WindowClass::GetClassName() const
{
	return m_ClassName;
}

HINSTANCE Window::WindowClass::GetInstance() const
{
	return m_hInstance;
}
