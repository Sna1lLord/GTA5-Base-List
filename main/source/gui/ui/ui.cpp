#include "ui.hpp"
#include "../utility/logger/logger.hpp"
#include "../utility/rage/invoker/natives/natives.hpp"
#include "../../features/features.hpp"
#include "../utility/rage/invoker/queue.hpp"
#include "../utility/config/config.hpp"

static Ui* uiInstance = nullptr;
static Paint* paintInstance = nullptr;
static ScrollAnimation* scrollInstance = nullptr;

Paint* Paint::Instance() {
	if (!paintInstance)
		paintInstance = new Paint();

	return paintInstance;
}

ScrollAnimation* ScrollAnimation::Instance() {
	if (!scrollInstance)
		scrollInstance = new ScrollAnimation();

	return scrollInstance;
}

void ScrollAnimation::Uninitialize() {
    scrollInstance = nullptr;
    delete this;
}

void Paint::Initialize() {
	
}

void Paint::Uninitialize() {
    paintInstance = nullptr;
    delete this;
}

void Paint::AddRectFilled(float x, float y, float w, float h, float rounding, ImDrawFlags flags, ImU32 color, bool foreground) {
	ImDrawList* list = nullptr;
	if (foreground)
		list = ImGui::GetForegroundDrawList();
	else
		list = ImGui::GetBackgroundDrawList();

	list->AddRectFilled(
		ImVec2(x, y), 
		ImVec2(x + w, y + h), 
		color, 
		rounding, 
		flags
	);
}

void Paint::AddRect(float x, float y, float w, float h, float rounding, float thickness, ImU32 color) {
	ImGui::GetForegroundDrawList()->AddRect(
		ImVec2(x, y), 
		ImVec2(x + w, y + h), 
		color, 
		rounding, 
		0,
		thickness
	);
}

void Paint::AddRect2(float x1, float y1, float x2, float y2, float rounding, float thickness, ImU32 color) {
	ImGui::GetForegroundDrawList()->AddRect(
		ImVec2(x1, y1), 
		ImVec2(x2, y2), 
		color, 
		rounding, 
		0,
		thickness
	);
}

void Paint::AddText(float x, float y, ImU32 color, const char* text) {
	ImGui::GetForegroundDrawList()->AddText(
		ImVec2(x, y), 
		color, 
		text
	);
}

void Paint::AddImage(ID3D11ShaderResourceView* image, float x, float y, float w, float h, float rounding, ImDrawFlags flags, ImU32 color) {
	ImGui::GetForegroundDrawList()->AddImageRounded(
		image, 
		ImVec2(x, y), 
		ImVec2(w, h), 
		ImVec2(0, 0), 
		ImVec2(1, 1), 
		color,
		rounding,
		flags
	);
}

void Paint::DrawBounding() {
	if (!Ui::Instance()->stack.empty()) {
		int optionCount = Ui::Instance()->GetSubmenu()->GetOptionCount();
		int end = optionCount > config.pageSize ? config.pageSize : optionCount;

		AddRect(
			config.positionX,
			config.positionY,
			config.width,
			config.headerHeight + config.barHeight + (end * config.optionHeight) + config.barHeight,
			config.menuRounding,
			config.boundingThickness,
			colors.boundingColor
		);
	}
}

void Paint::DrawHeader(const char* text) {
	AddRectFilled(
		config.positionX,
		config.positionY,
		config.width,
		config.headerHeight,
		config.menuRounding,
		ImDrawFlags_RoundCornersTop,
		colors.headerBackgroundColor
	);

	if (texture != nullptr) {
		Paint::Instance()->AddImage(
			texture,
			config.positionX,
			config.positionY,
			config.width + 50.f,
			config.headerHeight + 50.f,
			config.menuRounding,
			ImDrawFlags_RoundCornersTop,
			ImColor(255, 255, 255, 255)
		);

		return;
	}

	ImGui::PushFont(Ui::Instance()->fonts.titleFont);

	AddText(
		config.positionX + ((config.width - ImGui::CalcTextSize(text).x) / 2),
		config.positionY + 35.f,
		colors.headerTextColor,
		text
	);

	ImGui::PopFont();
}

void Paint::DrawBars() {
	AddRectFilled(
		config.positionX,
		config.positionY + config.headerHeight,
		config.width,
		config.barHeight,
		0.f,
		0,
		colors.barBackgroundColor
	);

	if (!Ui::Instance()->stack.empty()) {
		std::shared_ptr<Submenu> submenu = Ui::Instance()->GetSubmenu();

		int optionCount = submenu->GetOptionCount();
		int currentOption = submenu->GetCurrentOption();
		int end = optionCount > config.pageSize ? config.pageSize : optionCount;

		float footerY = config.positionY + config.headerHeight + config.barHeight + (end * config.optionHeight);
		AddRectFilled(
			config.positionX,
			footerY,
			config.width,
			config.barHeight,
			config.menuRounding,
			ImDrawFlags_RoundCornersBottom,
			colors.barBackgroundColor
		);

		if (currentOption != 0) {
			AddRectFilled(
				config.positionX,
				config.positionY + config.headerHeight + config.barHeight,
				config.width,
				2.f,
				0.f,
				0,
				colors.smallBarBackgroundColor,
				true
			);
		}

		if ((currentOption + 1) != (optionCount < config.pageSize ? optionCount : config.pageSize)) {
			AddRectFilled(
				config.positionX,
				footerY - 2.f,
				config.width,
				2.f,
				0.f,
				0,
				colors.smallBarBackgroundColor,
				true
			);
		}

		const char* footerText = "";
		if (currentOption == 0)
			footerText = ";";
		else if (currentOption + 1 == optionCount)
			footerText = ":";
		else
			footerText = "=";

		ImGui::PushFont(Ui::Instance()->fonts.footerFont);

		AddText(
			config.positionX + (config.width / 2) - ImGui::CalcTextSize(footerText).x,
			footerY + 8.f,
			colors.barTextColor,
			footerText
		);

		ImGui::PopFont();

		std::string text = std::format("{}/{}", submenu->GetCurrentOption() + 1, optionCount);

		ImGui::PushFont(Ui::Instance()->fonts.barFont);

		AddText(
			config.positionX + config.width - ImGui::CalcTextSize(text.c_str()).x - 10.f,
			config.positionY + config.headerHeight + 7.3f,
			colors.barTextColor,
			text.c_str()
		);

		AddText(
			config.positionX + 10.f,
			config.positionY + config.headerHeight + 7.f,
			colors.barTextColor,
			submenu->text
		);

		ImGui::PopFont();
	}
}

void Paint::DrawOption(std::shared_ptr<Submenu> submenu, int i, int j, int end, bool selected) {
	auto option = submenu->GetOption(i);

	AddRectFilled(
		config.positionX,
		config.positionY + config.headerHeight + config.barHeight + (j * config.optionHeight),
		config.width,
		config.optionHeight,
		0.f,
		0,
		colors.optionUnselected
	);

	AddRectFilled(
		config.positionX,
		ScrollAnimation::Instance()->GetCurrent(),
		config.width,
		config.optionHeight,
		0.f,
		0,
		colors.optionSelectedBackgroundColor
	);

	AddRectFilled(
		config.positionX,
		ScrollAnimation::Instance()->GetCurrent(),
		config.width,
		2.f,
		0.f,
		0,
		colors.optionSelected
	);

	AddRectFilled(
		config.positionX,
		ScrollAnimation::Instance()->GetCurrent() + config.optionHeight - 2.f,
		config.width,
		2.f,
		0.f,
		0,
		colors.optionSelected
	);

	if (selected) {
        option->Execute(ActionType::Hover);

		float targetPosition = config.positionY + config.headerHeight + config.barHeight + (j * config.optionHeight);
		ScrollAnimation::Instance()->Initialize(targetPosition, config.scrollerAnimationSpeed);
	}

	const char* left = option->GetLeftText();
	if (!(option->type == 3)) {
		AddText(
			config.positionX + 10.f,
			config.positionY + config.headerHeight + config.barHeight + (j * config.optionHeight) + ((config.optionHeight - ImGui::CalcTextSize(left).y) / 2),
			selected ? colors.optionSelectedText : colors.optionUnselectedText,
			left
		);
	}
	else {
		AddRect(
			config.positionX + 10.f,
			config.positionY + config.headerHeight + config.barHeight + (j * config.optionHeight) + 23.f,
			config.width - 23.f,
			1.5f,
			0.f,
			1.f,
			colors.optionBreakText
		);
	}

	const char* right = option->GetRightText();
	const char* secondRight = option->GetSecondRightText();
	bool needsFont = false;

	if (strcmp(right, "+") == 0 || strcmp(right, "-") == 0 || strcmp(right, "!") == 0 || strcmp(right, "^") == 0 || strcmp(right, "¨") == 0)
		needsFont = true;
	
	if (needsFont) {
		if (strcmp(right, "!") == 0)
			ImGui::PushFont(Ui::Instance()->fonts.submenuFont);
		else
			ImGui::PushFont(Ui::Instance()->fonts.iconFont);
	}

	//Why didn't I just use CalcTextSize??
	if (strcmp(secondRight, "0") != 0) {
		if (strcmp(secondRight, "+ ~") == 0 || strcmp(secondRight, "- ~") == 0) {
			AddText(
				config.positionX + config.width - ImGui::CalcTextSize(right).x - 85.f,
				config.positionY + config.headerHeight + config.barHeight + (j * config.optionHeight) + ((config.optionHeight - ImGui::CalcTextSize(right).y) / 2),
				selected ? colors.optionSelectedText : colors.optionUnselectedText,
				right
			);
		}
		else {
			AddText(
				config.positionX + config.width - ImGui::CalcTextSize(right).x - 45.f,
				config.positionY + config.headerHeight + config.barHeight + (j * config.optionHeight) + ((config.optionHeight - ImGui::CalcTextSize(right).y) / 2),
				selected ? colors.optionSelectedText : colors.optionUnselectedText,
				right
			);
		}

		ImGui::PushFont(Ui::Instance()->fonts.iconFont);

		AddText(
			config.positionX + config.width - ImGui::CalcTextSize(secondRight).x - 10.f,
			config.positionY + config.headerHeight + config.barHeight + (j * config.optionHeight) + ((config.optionHeight - ImGui::CalcTextSize(secondRight).y) / 2),
			selected ? colors.optionSelectedText : colors.optionUnselectedText,
			secondRight
		);

		ImGui::PopFont();
	}
	else {
		AddText(
			config.positionX + config.width - ImGui::CalcTextSize(right).x - 10.f,
			config.positionY + config.headerHeight + config.barHeight + (j * config.optionHeight) + ((config.optionHeight - ImGui::CalcTextSize(right).y) / 2),
			selected ? colors.optionSelectedText : colors.optionUnselectedText,
			right
		);

		if (option->type == 4) {
			AddRectFilled(
				config.positionX + config.width - ImGui::CalcTextSize(right).x - 40.f,
				config.positionY + config.headerHeight + config.barHeight + (j * config.optionHeight) + (config.optionHeight / 2 - 5.f),
				10.f,
				10.f,
				0.f,
				0,
				Config::Instance()->FloatToImColor(option->colour),
				true
			);

			if (option->colour[0] == colors.optionUnselected.Value.x &&
				option->colour[1] == colors.optionUnselected.Value.y &&
				option->colour[2] == colors.optionUnselected.Value.z &&
				option->colour[3] == colors.optionUnselected.Value.w) {
					AddRect(
						config.positionX + config.width - ImGui::CalcTextSize(right).x - 40.f,
						config.positionY + config.headerHeight + config.barHeight + (j * config.optionHeight) + (config.optionHeight / 2 - 5.f),
						10.f,
						10.f,
						0.f,
						1.f,
						ImColor(255, 255, 255, 255)
					);
				} else if (option->colour[0] == colors.optionSelectedBackgroundColor.Value.x &&
							option->colour[1] == colors.optionSelectedBackgroundColor.Value.y &&
							option->colour[2] == colors.optionSelectedBackgroundColor.Value.z &&
							option->colour[3] == colors.optionSelectedBackgroundColor.Value.w) {
								AddRect(
									config.positionX + config.width - ImGui::CalcTextSize(right).x - 40.f,
									config.positionY + config.headerHeight + config.barHeight + (j * config.optionHeight) + (config.optionHeight / 2 - 5.f),
									10.f,
									10.f,
									0.f,
									1.f,
									ImColor(32, 32, 32, 255)
								);
							}
		}
	}

	if (needsFont)
		ImGui::PopFont();

	if (selected) {
		ImGui::PushFont(Ui::Instance()->fonts.informationFont);

		AddRectFilled(
			config.positionX,
			config.positionY + config.headerHeight + config.barHeight + (end * config.optionHeight) + config.barHeight + 15.f,
			5.f,
			35.f,
			10.f,
			ImDrawFlags_RoundCornersLeft,
			colors.descriptionSecondaryColor,
			true
		);

		const char* description = option->GetDescription();
		if (description != "") {
			AddRectFilled(
				config.positionX + 5.f,
				config.positionY + config.headerHeight + config.barHeight + (end * config.optionHeight) + config.barHeight + 15.f,
				config.width - 5.f,
				35.f,
				10.f,
				ImDrawFlags_RoundCornersRight,
				colors.descriptionBackgroundColor
			);

			AddText(
				config.positionX + 15.f,
				config.positionY + config.headerHeight + config.barHeight + (end * config.optionHeight) + config.barHeight + 21.f,
				colors.descriptionTextColor,
				description
			);
		}

		ImGui::PopFont();

		ImGui::PushFont(Ui::Instance()->fonts.informationFont2);

		AddText(
			config.positionX + config.width - 26.f,
			config.positionY + config.headerHeight + config.barHeight + (end * config.optionHeight) + config.barHeight + 23.f,
			colors.descriptionTextColor,
			"?"
		);

		ImGui::PopFont();
	}
}

void Paint::DrawNotifications() {
	for (int i = 0; i < (int)notifications.size(); i++) {
		if (GetTickCount64() - notifications[i].second >= 3000) {
			notifications.erase(notifications.begin() + i);
			continue;
		}

		ImVec2 screenSize = ImGui::GetIO().DisplaySize;

		ImGui::PushFont(Ui::Instance()->fonts.notificationFont);

		float width = ImGui::CalcTextSize(notifications[i].first.c_str()).x + 40.f;
		float positionY = 30.f + (i * 43.f);
		float barWidth = width - (float)(((double)(GetTickCount64() - notifications[i].second) / 3000) * width);

		AddRectFilled(
			screenSize.x - width,
			positionY,
			width,
			30.f,
			0.f,
			0,
			colors.notificationBackgroundColor
		);

		AddRectFilled(
			screenSize.x - width,
			positionY,
			5.f,
			30.f,
			0.f,
			0,
			colors.notificationBarColor
		);

		AddRectFilled(
			screenSize.x - width,
			positionY + 30.f,
			barWidth,
			5.f,
			0.f,
			0,
			colors.notificationBarColor
		);

		AddText(
			screenSize.x - width + 25.f,
			positionY + 4.f,
			colors.notificationTextColor,
			notifications[i].first.c_str()
		);

		ImGui::PopFont();
	}
}

void Paint::Tick() {
    config = Ui::Instance()->config;
    colors = Ui::Instance()->colors;
}

void Paint::DrawInput() {
	
}

Ui* Ui::Instance() {
	if (!uiInstance)
		uiInstance = new Ui();

	return uiInstance;
}

void Ui::Initialize() {
	KEY(keys.open) = VK_F4;
	DELAY(keys.open) = 20;
	SET_PRESSED(keys.open, false);

	KEY(keys.enter) = VK_RETURN;
	DELAY(keys.enter) = 20;
	SET_PRESSED(keys.enter, false);

	KEY(keys.back) = VK_BACK;
	DELAY(keys.back) = 20;
	SET_PRESSED(keys.back, false);

	KEY(keys.up) = VK_UP;
	DELAY(keys.up) = 10;
	SET_PRESSED(keys.up, false);

	KEY(keys.down) = VK_DOWN;
	DELAY(keys.down) = 10;
	SET_PRESSED(keys.down, false);

	KEY(keys.left) = VK_LEFT;
	DELAY(keys.left) = 10;
	SET_PRESSED(keys.left, false);

	KEY(keys.right) = VK_RIGHT;
	DELAY(keys.right) = 10;
	SET_PRESSED(keys.right, false);

	//__________________

	config.pageSize = 12;
	config.menuRounding = 7.f;

	config.positionX = 50.f;
	config.positionY = 50.f;
	config.width = 520.f;

	config.boundingThickness = 2.f;

	config.optionHeight = 50.f;
	config.headerHeight = 128.f;
	config.barHeight = 35.f;

	config.sound = true;
	config.drawBounding = true;
	config.scrollerAnimationSpeed = 0.05f;

	//__________________
	
	colors.boundingColor = ImColor(0,102,255, 255);

	colors.headerBackgroundColor = ImColor(32, 32, 32, 255);
	colors.headerTextColor = ImColor(255, 255, 255, 255);

	colors.smallBarBackgroundColor = ImColor(0, 102, 255, 255);
	colors.barBackgroundColor = ImColor(32, 32, 32, 255);
	colors.barTextColor = ImColor(255, 255, 255, 255);

	colors.optionSelectedBackgroundColor = ImColor(100, 100, 100, 255);
	colors.optionSelected = ImColor(0, 102, 255, 255);
	colors.optionUnselected = ImColor(32, 32, 32, 255);
	colors.optionSelectedText = ImColor(255, 255, 255, 255);
	colors.optionUnselectedText = ImColor(255, 255, 255, 255);
	colors.optionBreakText = ImColor(255, 255, 255, 255);

	colors.descriptionTextColor = ImColor(255, 255, 255, 255);
	colors.descriptionBackgroundColor = ImColor(32, 32, 32, 255);
	colors.descriptionSecondaryColor = ImColor(0, 102, 255, 255);

	colors.notificationTextColor = ImColor(255, 255, 255, 255);
	colors.notificationBackgroundColor = ImColor(32, 32, 32, 255);
	colors.notificationBarColor = ImColor(0, 102, 255, 255);

	colors.informationTextColor = ImColor(255, 255, 255, 255);
	colors.informationBackgroundColor = ImColor(32, 32, 32, 255);
	colors.informationSecondaryColor = ImColor(0, 102, 255, 255);

	colors.pedPreviewBackgroundColor = ImColor(32, 32, 32, 255);
	colors.pedPreviewBorderColor = ImColor(0, 102, 255, 255);
}

void Ui::Uninitialize() {
    uiInstance = nullptr;
    delete this;
}

void Ui::Tick() {
	Ui::Instance()->Input();
	Ui::Instance()->Handler();
	Paint::Instance()->DrawInput();

	if (!stack.empty()) {
		std::shared_ptr<Submenu> submenu = GetSubmenu();
		submenu->options.clear();

		if (opened) {
			Paint::Instance()->DrawHeader("BASE");

			submenu->Execute();

			int optionCount = submenu->GetOptionCount();
			int currentOption = submenu->GetCurrentOption();

			if (optionCount != 0) {
				int start = 0;
				int end = optionCount > config.pageSize ? config.pageSize : optionCount;
				int menuEnd = optionCount > config.pageSize ? config.pageSize : optionCount;

				if (optionCount > config.pageSize && currentOption >= config.pageSize) {
					start = currentOption - config.pageSize + 1;
					end = currentOption + 1;
				}

				ImGui::PushFont(fonts.optionFont);

				for (int i = start, j = 0; i < end; i++, j++) {
					Paint::Instance()->DrawOption(
						submenu, 
						i,
						j,
						menuEnd,
						i == currentOption
					);
				}

				ImGui::PopFont();
			}

			if (config.drawBounding)
				Paint::Instance()->DrawBounding();
			Paint::Instance()->DrawBars();
			ScrollAnimation::Instance()->Update();
		}
	}

	Ui::Instance()->Reset();
	Paint::Instance()->DrawNotifications();
}

uint64_t tick = 0;

void Ui::Press(key& theKey) {
	if (GetAsyncKeyState(KEY(theKey)) & 1) {
		SET_PRESSED(theKey, true);
	}
	else if (GetAsyncKeyState(KEY(theKey)) & 0x8000) {
		if (GetTickCount64() - tick > DELAY(theKey) * 100) {
			SET_PRESSED(theKey, true);
		}
	}
	else {
		tick = GetTickCount64();
	}
}

void PlayFrontendSound(const char* sound) {
	Queue::Instance()->Add([sound]{
		AUDIO::PLAY_SOUND_FRONTEND(-1, sound, "HUD_FRONTEND_DEFAULT_SOUNDSET", 0);
	});
}

void Ui::Input() {
    Press(keys.open);
	Press(keys.enter);
	Press(keys.back);
	Press(keys.up);
	Press(keys.down);
	Press(keys.left);
	Press(keys.right);
}

void Ui::Handler() {
	if (PRESSED(keys.open)) {
		if (config.sound)
			PlayFrontendSound("SELECT");

		opened ^= true;
	}

	if (stack.empty() || drawingInput)
		return;
	
	if (opened) {
		std::shared_ptr<Submenu> submenu = GetSubmenu();

		if (PRESSED(keys.back)) {
			if (stack.size() > 1 && submenu->text != "Home") {
				if (config.sound)
					PlayFrontendSound("BACK");
				
				stack.pop();
			}
		}

		if (PRESSED(keys.up)) {
			if (config.sound)
				PlayFrontendSound("NAV_UP_DOWN");

			submenu->Scroll(false);
		}

		if (PRESSED(keys.down)) {
			if (config.sound)
				PlayFrontendSound("NAV_UP_DOWN");
				
			submenu->Scroll(true);
		}

		if (submenu->GetOptionCount() != 0) {
			auto option = submenu->GetOption(submenu->GetCurrentOption());

			if (option) {
				if (PRESSED(keys.enter)) {
					if (config.sound)
						PlayFrontendSound("SELECT");

					option->Execute(ActionType::Enter);
				}
				if (PRESSED(keys.left)) {
					if (config.sound)
						PlayFrontendSound("NAV_LEFT_RIGHT");
						
					option->Execute(ActionType::Left);
				}
				if (PRESSED(keys.right)) {
					if (config.sound)
						PlayFrontendSound("NAV_LEFT_RIGHT");

					option->Execute(ActionType::Right);
				}
			}
		}
	}
}

void Ui::Reset() {
	SET_PRESSED(keys.open, false);
	SET_PRESSED(keys.enter, false);
	SET_PRESSED(keys.back, false);
	SET_PRESSED(keys.up, false);
	SET_PRESSED(keys.down, false);
	SET_PRESSED(keys.left, false);
	SET_PRESSED(keys.right, false);
}

void Ui::AddSubmenu(const char* name, std::function<void(Submenu*)> function) {
	std::shared_ptr<Submenu> submenu = std::make_shared<Submenu>(name, function);
	if (stack.empty())
		stack.push(submenu);
	else {
		vector.push_back(std::move(submenu));
	}
}

void Ui::SwitchSubmenu(const char* name) {
	for (auto submenu : vector) {
		if (strcmp(submenu->text, name) == 0) {
			stack.push(submenu);
			return;
		}
	}
}

std::shared_ptr<Submenu> Ui::GetSubmenu() {
	return stack.top();
}