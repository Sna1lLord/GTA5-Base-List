#ifndef _UI_HPP
#define _UI_HPP

#include <common.hpp>
#include "../utility/rage/rage.hpp"

class Submenu;

using key = std::tuple<int8_t, int, bool>;
#define KEY(key) (std::get<0>(key))
#define DELAY(key) (std::get<1>(key))
#define PRESSED(key) (std::get<2>(key))
#define SET_PRESSED(key, value) (std::get<2>(key) = (value))

struct fonts_t {
	ImFont* titleFont;
	ImFont* optionFont;
	ImFont* barFont;
	ImFont* footerFont;
	ImFont* informationFont;
	ImFont* informationFont2;
	ImFont* submenuFont;
	ImFont* iconFont;
	ImFont* notificationFont;
	ImFont* inputFont;
};

struct keys_t {
	key open;
	key enter;
	key back;
	key up;
	key down;
	key left;
	key right;
};

struct config_t {
	int pageSize;
	float menuRounding;

	float positionX;
	float positionY;
	float width;

	float boundingThickness;

	float optionHeight;
	float headerHeight;
	float barHeight;

	bool sound;
	bool drawBounding;
	float scrollerAnimationSpeed;
};

struct colors_t {
	ImColor boundingColor;

	ImColor headerBackgroundColor;
	ImColor headerTextColor;

	ImColor smallBarBackgroundColor;
	ImColor barBackgroundColor;
	ImColor barTextColor;

	ImColor optionSelectedBackgroundColor;
	ImColor optionSelected;
	ImColor optionUnselected;
	ImColor optionSelectedText;
	ImColor optionUnselectedText;
	ImColor optionBreakText;

	ImColor descriptionTextColor;
	ImColor descriptionBackgroundColor;
	ImColor descriptionSecondaryColor;

	ImColor notificationTextColor;
	ImColor notificationBackgroundColor;
	ImColor notificationBarColor;

	ImColor informationTextColor;
	ImColor informationBackgroundColor;
	ImColor informationSecondaryColor;

	ImColor pedPreviewBackgroundColor;
	ImColor pedPreviewBorderColor;
};

class ScrollAnimation {
public:
	float target;
	float current;
	float speed;
public:
	static ScrollAnimation* Instance();
public:
	void Initialize(float targetPosition, float scrollSpeed) {
		target = targetPosition;
		speed = scrollSpeed;
	}
    void Uninitialize();
	void Update() {
		if (current != target) {
			current = current * (1 - speed) + target * speed;
		}
		else if ((current > target - 0.0005) && (current < target + 0.0005))
			current = target;
	}
	void SetTarget(float newTarget) {
		target = newTarget;
	}
	float GetCurrent() {
		return current;
	}
};

class Paint {
public:
    config_t config;
    colors_t colors;

	ID3D11ShaderResourceView* texture;
	std::vector<std::pair<std::string, uint64_t>> notifications;
public:
	static Paint* Instance();
	void Initialize();
    void Uninitialize();
public:
	void AddRectFilled(float x, float y, float w, float h, float rounding, ImDrawFlags flags, ImU32 color, bool foreground = false);
	void AddRect(float x, float y, float w, float h, float rounding, float thickness, ImU32 color);
	void AddRect2(float x1, float y1, float x2, float y2, float rounding, float thickness, ImU32 color);
	void AddText(float x, float y, ImU32 color, const char* text);
	void AddImage(ID3D11ShaderResourceView* image, float x, float y, float w, float h, float rounding, ImDrawFlags flags, ImU32 color);

	void DrawBounding();
	void DrawHeader(const char* text);
	void DrawBars();
	void DrawOption(std::shared_ptr<Submenu> submenu, int i, int j, int end, bool selected);

	void AddNotification(const char* format, ...) {
		char notificationText[256];

		va_list args;
		va_start(args, format);
		vsnprintf(notificationText, sizeof(notificationText), format, args);

		for (auto pair : notifications) {
			if (pair.first == std::string(notificationText)) {
				pair.second = GetTickCount64();
				return;
			}
		}

		notifications.push_back(std::make_pair(std::string(notificationText), GetTickCount64()));

		va_end(args);
	}

    void Tick();
	void DrawNotifications();
	void DrawInput();
};

class Ui {
public:
	std::vector<std::shared_ptr<Submenu>> vector;
    std::stack<std::shared_ptr<Submenu>> stack;

	fonts_t fonts;
	keys_t keys;
	config_t config;
	colors_t colors;
	bool opened;
	bool drawingInput;
public:
    static Ui* Instance();
public:
	void Initialize();
    void Uninitialize();
	void Tick();
	void Press(key& theKey);
	void Input();
    void Handler();
	void Reset();
public:
	void AddSubmenu(const char* name, std::function<void(Submenu*)> function);
	void SwitchSubmenu(const char* name);
	std::shared_ptr<Submenu> GetSubmenu();
};

enum ActionType : int {
	Enter = 0,
	Left,
	Right,
    Hover
};

class BaseOption {
public:
	BaseOption(const char* optionText, const char* optionDescription, std::function<void()> optionAction = []{}, std::function<void()> hoverAction = []{}, int optionType = 0, float* optionColour = nullptr) {
		left = optionText;
		description = optionDescription;
		action = optionAction;
        hover = hoverAction;
		type = optionType;
		colour = optionColour;
	}

	virtual void Execute(ActionType actionType) {
		if (actionType == ActionType::Enter)
			std::invoke(action);
        else if (actionType == ActionType::Hover)
			std::invoke(hover);
	}
	virtual const char* GetRightText() {
		return "";
	}
	virtual const char* GetSecondRightText() {
		return "0";
	}
	const char* GetLeftText() {
		return left.c_str();
	}
	const char* GetDescription() {
		return description.c_str();
	}
public:
    std::string left;
	std::string description;
	std::function<void()> action;
    std::function<void()> hover;
	int type;
	float* colour;
};

class RegularOption : public BaseOption {
public:
	RegularOption(const char* optionText, const char* optionDescription, std::function<void()> optionAction = []{})
        : BaseOption(optionText, optionDescription, optionAction, []{}, 1) {
    }
};

class InputOption : public BaseOption {
public:
	InputOption(const char* optionText, const char* optionDescription, char* inputOption, size_t inputOptionSize, std::function<void()> optionAction = []{})
		: BaseOption(optionText, optionDescription) {
			realAction = optionAction;
			action = [=]{
				Ui::Instance()->drawingInput = true;
			};

			hover = [=]{
				if (Ui::Instance()->drawingInput) {
					ImGui::GetIO().MouseDrawCursor = true;

					ImGui::SetNextWindowSize(ImVec2(200, 37));
					ImGui::SetNextWindowPos(ImVec2(Paint::Instance()->config.positionX + Paint::Instance()->config.width + 15.f, Paint::Instance()->config.positionY));
					ImGui::PushFont(Ui::Instance()->fonts.inputFont);
					ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);

					if (ImGui::Begin("##Input", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
						ImGui::InputText("##", input, inputSize);
						ImGui::SameLine();
						if (ImGui::Button("OK", ImVec2(45, 23))) {
							if (!(GetAsyncKeyState(VK_BACK))) {
								Ui::Instance()->drawingInput = false;
								std::invoke(realAction);
							}
							else {
								if (!(GetAsyncKeyState(VK_BACK))) {
									Ui::Instance()->drawingInput = false;
									std::invoke(realAction);
								}
							}
						}

						ImGui::End();
					}

					ImGui::PopFont();
					ImGui::PopStyleVar();
				}
				else {
					ImGui::GetIO().MouseDrawCursor = false;
				}
			};

			input = inputOption;
			inputSize = inputOptionSize;
    	}

	const char* GetRightText() override {
		return "¨";
	}
public:
	std::function<void()> realAction;
	char* input;
	size_t inputSize;
};

class PaletteOption : public BaseOption {
public:
	PaletteOption(const char* optionText, const char* optionDescription, float* colourOption, std::function<void()> optionAction = []{})
		: BaseOption(optionText, optionDescription, []{}, []{}, 4, colourOption) {
			realAction = optionAction;
			action = [=]{
				Ui::Instance()->drawingInput = true;
			};

			hover = [=]{
				if (Ui::Instance()->drawingInput) {
					ImGui::GetIO().MouseDrawCursor = true;

					ImGui::SetNextWindowSize(ImVec2(90, 37));
					ImGui::SetNextWindowPos(ImVec2(Paint::Instance()->config.positionX + Paint::Instance()->config.width + 15.f, Paint::Instance()->config.positionY));
					ImGui::PushFont(Ui::Instance()->fonts.inputFont);
					ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);

					if (ImGui::Begin("##Input", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
						ImGui::ColorEdit4("##", colour, ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoInputs);

						ImGui::SameLine();
						if (ImGui::Button("OK", ImVec2(45, 23))) {
							if (!(GetAsyncKeyState(VK_BACK))) {
								Ui::Instance()->drawingInput = false;
								std::invoke(realAction);
							}
							else {
								if (!(GetAsyncKeyState(VK_BACK))) {
									Ui::Instance()->drawingInput = false;
									std::invoke(realAction);
								}
							}
						}

						ImGui::End();
					}

					ImGui::PopFont();
					ImGui::PopStyleVar();
				}
				else {
					ImGui::GetIO().MouseDrawCursor = false;
				}
			};
		}

	const char* GetRightText() override {
		return "^";
	}
public:
	std::function<void()> realAction;
};

class BoolOption : public BaseOption {
public:
	BoolOption(const char* optionText, const char* optionDescription, bool* optionBool, std::function<void()> optionAction = []{})
        : BaseOption(optionText, optionDescription, optionAction, []{}, 2) {
		enabled = optionBool;
    }

	void Execute(ActionType actionType) override {
		if (actionType == ActionType::Enter) {
			*enabled ^= true;

			std::invoke(action);
		}
	}
	const char* GetRightText() override {
		if (*enabled)
			return "+";
		else
			return "-";
	}
public:
	bool* enabled;
};

template <class T>
class NumberOption : public BaseOption {
private:
	void Update() {
		if constexpr (std::is_floating_point_v<T>) {
			right = std::format("{:.1f}", *number);
		} 
		else {
			right = std::format("{}", *number);
		}
	}
public:
	NumberOption(const char* optionText, const char* optionDescription, T* optionNumber, T optionMin, T optionMax, T optionStep = 1, std::function<void()> optionAction = []{})
        : BaseOption(optionText, optionDescription, optionAction) {

		number = optionNumber;
		min = optionMin;
		max = optionMax;
		step = optionStep;

		Update();
    }

	void Execute(ActionType actionType) override {
		if (actionType == ActionType::Left) {
			if (!(*number - step < min) && !(*number - step + step/2 < min))
				*number -= step;
			else
				*number = min;

			std::invoke(action);
		}
		else if (actionType == ActionType::Right) {
			if (!(*number + step > max) && !(*number + step - step/2 > max))
				*number += step;
			else
				*number = max;

			std::invoke(action);
		}
		else if (actionType == ActionType::Enter) {
			std::invoke(action);
		}

		Update();
	}
	const char* GetRightText() override {
		return right.c_str();
	}
	const char* GetSecondRightText() override {
		return "~";
	}
public:
	T* number;
	T min;
	T max;
	T step;
	std::string right;
};

class SubOption : public BaseOption {
public:
	SubOption(const char* optionText, const char* optionDescription, const char* submenuName, std::function<void()> optionAction = []{})
		: BaseOption(optionText, optionDescription, optionAction) {

		submenu = submenuName;
	}

	void Execute(ActionType actionType) override {
		if (actionType == ActionType::Enter) {
			std::invoke(action);

			Ui::Instance()->SwitchSubmenu(submenu);
		}
	}
	const char* GetRightText() override {
		return "!";
	}
public:
	const char* submenu;
};

class Submenu {
public:
    Submenu(const char* name, std::function<void(Submenu*)> function) {
        text = name;
        currentOption = 0;
		action = function;
    }

	void Execute() {
		std::invoke(action, static_cast<Submenu*>(this));
	}

    int GetOptionCount() {
		return (int)options.size();
	}

	int GetCurrentOption() {
		return currentOption;
	}

	template <typename T, typename ...Args>
	void AddOption(Args&&... args) {
		std::shared_ptr<T> option = std::make_shared<T>(std::forward<Args>(args)...);
		options.push_back(option);
	}

	std::shared_ptr<BaseOption> GetOption(size_t i) {
		if (i < (size_t)GetOptionCount())
			return options[i];
		else
			return nullptr;
	}

	void Scroll(bool down) {
		if (down) {
			if (currentOption < GetOptionCount() - 1) {
				if (GetOption(currentOption + 1)->type == 3)
					currentOption += 2;
				else
					currentOption += 1;
			}
		}
		else {
			if (currentOption != 0) {
				if (GetOption(currentOption - 1)->type == 3)
					currentOption -= 2;
				else
					currentOption -= 1;
			}
		}
	}
public:
	std::function<void(Submenu*)> action;
    std::vector<std::shared_ptr<BaseOption>> options;
    const char* text;
    int currentOption;
};

#endif