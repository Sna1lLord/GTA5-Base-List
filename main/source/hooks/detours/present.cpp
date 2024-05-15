#include "../hooks.hpp"
#include "../utility/logger/logger.hpp"
#include "../source/main.hpp"
#include "../../gui/gui.hpp"
#include "../../gui/ui/ui.hpp"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (running) {
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
    }

	return CallWindowProc(Hooks::Instance()->originalWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall Hooks::Present(IDXGISwapChain* swapChain, UINT syncInterval, UINT flags) {
    if (!Gui::Instance()->initialized) {
        if (SUCCEEDED(swapChain->GetDevice(__uuidof(ID3D11Device), (void**)(&Gui::Instance()->device)))) {
			Gui::Instance()->device->GetImmediateContext(&Gui::Instance()->context);
            Gui::Instance()->hwnd = FindWindowA("grcWindow", nullptr);

            Hooks::Instance()->originalWndProc = (WNDPROC)SetWindowLongPtr(Gui::Instance()->hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

			ImGui::CreateContext();
            Gui::Instance()->Initialize();

            ImGui_ImplDX11_Init(Gui::Instance()->device, Gui::Instance()->context);
            ImGui_ImplWin32_Init(Gui::Instance()->hwnd);

			Gui::Instance()->initialized = true;
		}
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    Ui::Instance()->Tick();

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return static_cast<decltype(&Present)>(Hooks::Instance()->originalPresent)(swapChain, syncInterval, flags);
}