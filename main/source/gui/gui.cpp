#include "gui.hpp"
#include "../utility/logger/logger.hpp"
#include "../source/hooks/hooks.hpp"
#include "../utility/rage/invoker/natives/natives.hpp"
#include "../utility/rage/invoker/queue.hpp"
#include "../utility/signatures/signatures.hpp"
#include "../utility/rage/joaat.hpp"
#include "ui/ui.hpp"

static Gui* guiInstance = nullptr;

Gui* Gui::Instance() {
	if (!guiInstance)
		guiInstance = new Gui();

	return guiInstance;
}

void Gui::Tick() {
	Paint::Instance()->Tick();

	if (Ui::Instance()->opened) {
        Ui::Instance()->AddSubmenu("Home", [](Submenu* submenu) {
			submenu->AddOption<SubOption>("Self", "Options that affect yourself", "Self", []{
				Self();
			});

			submenu->AddOption<SubOption>("Weapon", "Options that affect your weapons", "Weapon", []{
				Weapon();
			});

			submenu->AddOption<SubOption>("Vehicle", "Options that affect your vehicle", "Vehicle", []{
				Vehicle();
			});

			submenu->AddOption<SubOption>("Session", "Options that affect your current session", "Session", []{
				Session();
			});

			submenu->AddOption<SubOption>("Teleport", "Teleport around the map", "Teleport", []{
				Teleport();
			});

			submenu->AddOption<SubOption>("Recovery", "Modify xp, money etc", "Recovery", []{
				Recovery();
			});

			submenu->AddOption<SubOption>("World", "World options", "World", []{
				World();
			});

			submenu->AddOption<SubOption>("Protections", "Protections against modders", "Protections", []{
				Protections();
			});

			submenu->AddOption<SubOption>("Settings", "Change menu settings", "Settings", []{
				Settings();
			});
		});
    }
}

void Gui::Initialize() {
	ImGuiIO io = ImGui::GetIO();

    ImFontConfig config;
	config.FontDataOwnedByAtlas = false;
	Ui::Instance()->fonts.titleFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arialbd.ttf", 60.f, &config);
	Ui::Instance()->fonts.optionFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 22.f, &config);
	Ui::Instance()->fonts.barFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 19.f, &config);
	Ui::Instance()->fonts.footerFont = io.Fonts->AddFontFromMemoryTTF((void*)iconData, iconDataSize, 15.f, &config);
	Ui::Instance()->fonts.informationFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 20.f, &config);
	Ui::Instance()->fonts.informationFont2 = io.Fonts->AddFontFromMemoryTTF((void*)iconData, iconDataSize, 20.f, &config);
	Ui::Instance()->fonts.submenuFont = io.Fonts->AddFontFromMemoryTTF((void*)iconData, iconDataSize, 18.f, &config);
	Ui::Instance()->fonts.iconFont = io.Fonts->AddFontFromMemoryTTF((void*)iconData, iconDataSize, 24.f, &config);
	Ui::Instance()->fonts.notificationFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 18.f, &config);
	Ui::Instance()->fonts.inputFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 16.f, &config);

	ImVec4* colors = ImGui::GetStyle().Colors;
	ImGuiStyle style = ImGui::GetStyle();

	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.17647058823f, 0.20784313725f, 0.25098039215f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 1.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.f, 0.f, 0.f, 0.0f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.f, 0.f, 0.f, 0.0f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 1.00f, 0.96f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 1.00f, 0.96f, 0.42f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.00f, 1.00f, 0.96f, 0.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.f, 0.f, 0.f, 0.0f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.f, 0.f, 0.f, 0.0f);
	colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 0.50f);
	colors[ImGuiCol_TabActive] = ImVec4(0.27058823529f, 0.46274509803f, 0.74901960784f, 0.40f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.07f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

	style.WindowPadding = ImVec2(10, 10);
	style.FramePadding = ImVec2(15, 4);
	style.ItemSpacing = ImVec2(15, 5);
	style.ItemInnerSpacing = ImVec2(10, 4);
	style.ScrollbarSize = 5;
	style.GrabMinSize = 20;

	style.WindowBorderSize = 0;
	style.ChildBorderSize = 0;
	style.FrameBorderSize = 0;
	style.TabBorderSize = 0;

	style.WindowRounding = 5;
	style.ChildRounding = 12;
	style.FrameRounding = 10;
	style.ScrollbarRounding = 2;
	style.GrabRounding = 2;
	style.TabRounding = 2;

	style.WindowMenuButtonPosition = 0;
}

void Gui::Uninitialize() {
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (context) { context->Release(); context = NULL; }
	if (device) { device->Release(); device = NULL; }
	SetWindowLongPtr(Gui::Instance()->hwnd, GWLP_WNDPROC, (LONG_PTR)(Hooks::Instance()->originalWndProc));

	guiInstance = nullptr;
    delete this;
}