#include <thread>

#include "Stopwatch.h"
#include "Constants.h"
#include "Win32App.h"
#include "D3D11App.h"
#include "NightreignLayer.h"

#include "../ImGui/imgui.h"
#include "../ImGui/backends/imgui_impl_dx11.h"
#include "../ImGui/backends/imgui_impl_win32.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
	Win32Application overlay{ hInstance, nCmdShow };
	D3D11Application d3d11App{ overlay };
	NightreignLayer layer{ &overlay };

	// Full delta time (inc sleep & work done)
	float deltaTime = 0.0f;
	Stopwatch frameTimer{ false };

	// Actual work done (excludes sleep time)
	float workDeltaTime = 0.0f;
	Stopwatch workFrameTimer{ false };

	while (overlay.IsRunning())
	{
		frameTimer.Start();

		// Frame rate limiter
		// Sleep the CPU so we dont burn tones of cycles
		// FPS limiter needs to use the un-framelimited DT
		const float deltaTimeAsMS = (workDeltaTime * 1000.0f);
		if (deltaTime != 0.0f && deltaTimeAsMS < core::TargetMS && !layer.IsInEditMode())
		{
			std::chrono::duration<double, std::milli> delta_ms(core::TargetMS - workDeltaTime);
			auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
			std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
		}

		workFrameTimer.Start();

		// ===== Update Section =====

		overlay.MessagePump();
		layer.OnUpdate(deltaTime);

		// ===== End Update Section =====

		// ===== Render Section =====

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		d3d11App.ClearBackbuffer();
		layer.OnRender();

		ImGui::Render();

		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		d3d11App.Present();

		// ===== End Render Section =====

		workFrameTimer.Stop();
		workDeltaTime = workFrameTimer.fGetElapsedTime();

		frameTimer.Stop();
		deltaTime = frameTimer.fGetElapsedTime();
	}

	return 0;
}
