#include "pch.h"
#include "Win.h"
#include <d3d11.h>
#include "Window.h"

#include <cmath>

#include "Timer.h"
#include "Input.h"

#include "Core/GDIPlusManager.h"
#include "Core/ImGuiManager.h"
#include "Core/Surface.h"

#include "Renderer/Drawable/Box.h"
#include "Renderer/Drawable/Sheet.h"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC // to get more details
#include <crtdbg.h>
#endif

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
#ifdef _DEBUG
	_CrtMemState sOld;
	_CrtMemState sNew;
	_CrtMemState sDiff;
	_CrtMemCheckpoint(&sOld); //take a snapchot
#endif

	ImGuiManager imgui;
	GDIPlusManager gdipMan;

	const Surface s = Surface(L"D:\\dev\\C++\\DirectX\\DirectX\\electro.jpg");

	int ret;
	{
		Window wnd;
		Timer timer;

		Box box(wnd.Gfx(), 0.0f, 0.0f, 4.0f);
		Box box2(wnd.Gfx(), 4.0f, 0.0f, 12.0f);
		Sheet sheet(wnd.Gfx());
		float timescale = 1.0f;
		for (;;)
		{
			wnd.Gfx()->ClearRenderBuffer(1.0f, 1.0f, 1.0f, 1.0f);
			if (const auto retValue = wnd.Update())
			{
				ret = *retValue;
				break;
			}

			float time = timer.Reset();

			BoxVelocity& vel = box.Velocity();
			if (Input::IsKeyDown('W'))
			{
				vel.z += 0.0002f;
				vel.z = std::clamp(vel.z, -0.001f, 0.001f);
			}
			if (Input::IsKeyDown('S'))
			{
				vel.z -= 0.0002f;
				vel.z = std::clamp(vel.z, -0.001f, 0.001f);
			}

			if (!Input::IsKeyDown('W') && !Input::IsKeyDown('S'))
			{
				if (vel.z > 0.0f)
				{
					vel.z -= 0.0001f;
					if (vel.z < 0.0f)
						vel.z = 0.0f;
				}
				else if (vel.z < 0.0f)
				{
					vel.z += 0.0001f;
					if (vel.z > 0.0f)
						vel.z = 0.0f;
				}
			}

			sheet.Update(time * timescale);
			box.Update(time * timescale);
			box2.Update(time * timescale);

			box.Draw(wnd.Gfx());
			box2.Draw(wnd.Gfx());
			//sheet.Draw(wnd.Gfx());

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			if (ImGui::Begin("Timescale"))
			{
				ImGui::SliderFloat("Timescale", &timescale, 0.0f, 4.0f);
				ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}
			ImGui::End();

			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			wnd.Gfx()->Flush();
		}
	}

#ifdef _DEBUG
	_CrtMemCheckpoint(&sNew); //take a snapchot
	if (_CrtMemDifference(&sDiff, &sOld, &sNew)) // if there is a difference
	{
		OutputDebugString(L"-----------_CrtMemDumpStatistics ---------");
		_CrtMemDumpStatistics(&sDiff);
		OutputDebugString(L"-----------_CrtMemDumpAllObjectsSince ---------");
		_CrtMemDumpAllObjectsSince(&sOld);
		OutputDebugString(L"-----------_CrtDumpMemoryLeaks ---------");
		_CrtDumpMemoryLeaks();
	}
#endif

	return 0;
}
