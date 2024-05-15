#ifndef _CONFIG_HPP
#define _CONFIG_HPP

#include <common.hpp>
#include <ini.hpp>

#include "../source/gui/ui/ui.hpp"
#include "../utility/rage/invoker/natives/natives.hpp"
#include "../utility/logger/logger.hpp"

struct self_t {
    bool godmode = false;
};

struct settings_t {
    char filename[15] = "";

    float boundingColor[4];

    float headerBackgroundColor[4];
    float headerTextColor[4];

    float smallBarBackgroundColor[4];
    float barBackgroundColor[4];
    float barTextColor[4];

    float optionSelectedBackgroundColor[4];
    float optionSelected[4];
    float optionUnselected[4];
    float optionSelectedText[4];
    float optionUnselectedText[4];
    float optionBreakText[4];

    float descriptionTextColor[4];
    float descriptionBackgroundColor[4];
    float descriptionSecondaryColor[4];

    float notificationTextColor[4];
    float notificationBackgroundColor[4];
    float notificationBarColor[4];
};

class Config {
public:
    static Config* Instance();
    void Uninitialize();
public:
    ImColor FloatToImColor(float value[4]) {
        return ImColor(value[0], value[1], value[2], value[3]);
    }
    void ImColorToFloat(ImColor color, float value[4]) {
        value[0] = color.Value.x;
        value[1] = color.Value.y;
        value[2] = color.Value.z;
        value[3] = color.Value.w;
    }
    void SaveValue(const char* name, mINI::INIStructure& ini, float value[4]) {
        for (int i = 0; i < 4; i++) {
            ini["config"][std::format("{}_{}", name, i).c_str()] = std::to_string(value[i]);
        }
    }
    void SaveValue(const char* name, mINI::INIStructure& ini, float value) {
        ini["config"][name] = std::to_string(value);
    }
    void SaveValue(const char* name, mINI::INIStructure& ini, bool value) {
        ini["config"][name] = std::to_string(value);
    }
    void SaveValue(const char* name, mINI::INIStructure& ini, int value) {
        ini["config"][name] = std::to_string(value);
    }
    void Save(const char* name) {
        mINI::INIStructure ini;
        SaveValue("bounding_color", ini, settings.boundingColor);

        SaveValue("header_background_color", ini, settings.headerBackgroundColor);
        SaveValue("header_text_color", ini, settings.headerTextColor);

        SaveValue("small_bar_background_color", ini, settings.smallBarBackgroundColor);
        SaveValue("bar_background_color", ini, settings.barBackgroundColor);
        SaveValue("bar_text_color", ini, settings.barTextColor);

        SaveValue("option_selected_background_color", ini, settings.optionSelectedBackgroundColor);
        SaveValue("option_selected", ini, settings.optionSelected);
        SaveValue("option_unselected", ini, settings.optionUnselected);
        SaveValue("option_selected_text", ini, settings.optionSelectedText);
        SaveValue("option_unselected_text", ini, settings.optionUnselectedText);
        SaveValue("option_break_text", ini, settings.optionBreakText);

        SaveValue("description_text_color", ini, settings.descriptionTextColor);
        SaveValue("description_background_color", ini, settings.descriptionBackgroundColor);
        SaveValue("description_secondary_color", ini, settings.descriptionSecondaryColor);

        SaveValue("notification_text_color", ini, settings.notificationTextColor);
        SaveValue("notification_background_color", ini, settings.notificationBackgroundColor);
        SaveValue("notification_bar_color", ini, settings.notificationBarColor);

        SaveValue("sound", ini, Ui::Instance()->config.sound);
        SaveValue("draw_bounding", ini, Ui::Instance()->config.drawBounding);

        SaveValue("options_per_page", ini, Ui::Instance()->config.pageSize);
        SaveValue("rounding", ini, Ui::Instance()->config.menuRounding);
        SaveValue("position_x", ini, Ui::Instance()->config.positionX);
        SaveValue("position_y", ini, Ui::Instance()->config.positionY);
        SaveValue("menu_width", ini, Ui::Instance()->config.width);
        SaveValue("bounding_thickness", ini, Ui::Instance()->config.boundingThickness);
        SaveValue("option_height", ini, Ui::Instance()->config.optionHeight);
        SaveValue("header_height", ini, Ui::Instance()->config.headerHeight);
        SaveValue("bar_height", ini, Ui::Instance()->config.barHeight);

        mINI::INIFile file(std::format("{}\\Breeze\\Config\\{}", std::getenv("appdata"), name));
        file.write(ini);
    }
    void SaveOutfit(const char* name) {
        mINI::INIStructure ini;
        for (int i = 0; i < 12; i++) {
            SaveValue(std::format("clothing_model_{}", i).c_str(), ini, PED::GET_PED_DRAWABLE_VARIATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), i));
            SaveValue(std::format("clothing_variation_{}", i).c_str(), ini, PED::GET_PED_TEXTURE_VARIATION(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(PLAYER::PLAYER_ID()), i));
        }

        mINI::INIFile file(std::format("{}\\Breeze\\Outfit\\{}", std::getenv("appdata"), name));
        file.write(ini);
    }
    void LoadValue(const char* name, mINI::INIStructure& ini, ImColor& value) {
        float newValue[4];

        for (int i = 0; i < 4; i++) {
            std::string key = std::format("{}_{}", name, i);
            if (ini["config"].has(key)) {
                newValue[i] = std::stof(ini["config"][key]);
            }
        }

        value = FloatToImColor(newValue);
    }
    void LoadValue(const char* name, mINI::INIStructure& ini, float& value) {
        if (ini["config"].has(name)) {
            value = std::stof(ini["config"][name]);
        }
    }
    void LoadValue(const char* name, mINI::INIStructure& ini, bool& value) {
        if (ini["config"].has(name)) {
            value = (bool)std::stoi(ini["config"][name]);
        }
    }
    void LoadValue(const char* name, mINI::INIStructure& ini, int& value) {
        if (ini["config"].has(name)) {
            value = std::stoi(ini["config"][name]);
        }
    }
    void LoadValueV2(const char* name, mINI::INIStructure& ini, float value[4]) {
        for (int i = 0; i < 4; i++) {
            std::string key = std::format("{}_{}", name, i);
            if (ini["config"].has(key)) {
                value[i] = std::stof(ini["config"][key]);
            }
        }
    }
    void Load(const char* name) {
        mINI::INIFile file(std::format("{}\\Breeze\\Config\\{}", std::getenv("appdata"), name));
        mINI::INIStructure ini;

        if (file.read(ini)) {
            if (ini.has("config")) {
                LoadValue("bounding_color", ini, Ui::Instance()->colors.boundingColor);
                LoadValueV2("bounding_color", ini, settings.boundingColor);

                LoadValue("header_background_color", ini, Ui::Instance()->colors.headerBackgroundColor);
                LoadValue("header_text_color", ini, Ui::Instance()->colors.headerTextColor);
                LoadValueV2("header_background_color", ini, settings.headerBackgroundColor);
                LoadValueV2("header_text_color", ini, settings.headerTextColor);

                LoadValue("small_bar_background_color", ini, Ui::Instance()->colors.smallBarBackgroundColor);
                LoadValue("bar_background_color", ini, Ui::Instance()->colors.barBackgroundColor);
                LoadValue("bar_text_color", ini, Ui::Instance()->colors.barTextColor);
                LoadValueV2("small_bar_background_color", ini, settings.smallBarBackgroundColor);
                LoadValueV2("bar_background_color", ini, settings.barBackgroundColor);
                LoadValueV2("bar_text_color", ini, settings.barTextColor);

                LoadValue("option_selected_background_color", ini, Ui::Instance()->colors.optionSelectedBackgroundColor);
                LoadValue("option_selected", ini, Ui::Instance()->colors.optionSelected);
                LoadValue("option_unselected", ini, Ui::Instance()->colors.optionUnselected);
                LoadValue("option_selected_text", ini, Ui::Instance()->colors.optionSelectedText);
                LoadValue("option_unselected_text", ini, Ui::Instance()->colors.optionUnselectedText);
                LoadValue("option_break_text", ini, Ui::Instance()->colors.optionBreakText);
                LoadValueV2("option_selected_background_color", ini, settings.optionSelectedBackgroundColor);
                LoadValueV2("option_selected", ini, settings.optionSelected);
                LoadValueV2("option_unselected", ini, settings.optionUnselected);
                LoadValueV2("option_selected_text", ini, settings.optionSelectedText);
                LoadValueV2("option_unselected_text", ini, settings.optionUnselectedText);
                LoadValueV2("option_break_text", ini, settings.optionBreakText);

                LoadValue("description_text_color", ini, Ui::Instance()->colors.descriptionTextColor);
                LoadValue("description_background_color", ini, Ui::Instance()->colors.descriptionBackgroundColor);
                LoadValue("description_secondary_color", ini, Ui::Instance()->colors.descriptionSecondaryColor);
                LoadValueV2("description_text_color", ini, settings.descriptionTextColor);
                LoadValueV2("description_background_color", ini, settings.descriptionBackgroundColor);
                LoadValueV2("description_secondary_color", ini, settings.descriptionSecondaryColor);

                LoadValue("notification_text_color", ini, Ui::Instance()->colors.notificationTextColor);
                LoadValue("notification_background_color", ini, Ui::Instance()->colors.notificationBackgroundColor);
                LoadValue("notification_bar_color", ini, Ui::Instance()->colors.notificationBarColor);
                LoadValueV2("notification_text_color", ini, settings.notificationTextColor);
                LoadValueV2("notification_background_color", ini, settings.notificationBackgroundColor);
                LoadValueV2("notification_bar_color", ini, settings.notificationBarColor);

                LoadValue("sound", ini, Ui::Instance()->config.sound);
                LoadValue("draw_bounding", ini, Ui::Instance()->config.drawBounding);

                LoadValue("options_per_page", ini, Ui::Instance()->config.pageSize);
                LoadValue("options_per_page", ini, Ui::Instance()->config.pageSize);
                LoadValue("rounding", ini, Ui::Instance()->config.menuRounding);
                LoadValue("position_x", ini, Ui::Instance()->config.positionX);
                LoadValue("position_y", ini, Ui::Instance()->config.positionY);
                LoadValue("menu_width", ini, Ui::Instance()->config.width);
                LoadValue("bounding_thickness", ini, Ui::Instance()->config.boundingThickness);
                LoadValue("option_height", ini, Ui::Instance()->config.optionHeight);
                LoadValue("header_height", ini, Ui::Instance()->config.headerHeight);
                LoadValue("bar_height", ini, Ui::Instance()->config.barHeight);
            }
            else {
                Logger::Instance()->Log("Invalid Config File! \n");
            }
        }
        else {
            Logger::Instance()->Log("Invalid Config File! \n");
        }
    }
    void Initialize() {
        ImColorToFloat(Ui::Instance()->colors.boundingColor, settings.boundingColor);

        ImColorToFloat(Ui::Instance()->colors.headerBackgroundColor, settings.headerBackgroundColor);
        ImColorToFloat(Ui::Instance()->colors.headerTextColor, settings.headerTextColor);

        ImColorToFloat(Ui::Instance()->colors.smallBarBackgroundColor, settings.smallBarBackgroundColor);
        ImColorToFloat(Ui::Instance()->colors.barBackgroundColor, settings.barBackgroundColor);
        ImColorToFloat(Ui::Instance()->colors.barTextColor, settings.barTextColor);

        ImColorToFloat(Ui::Instance()->colors.optionSelectedBackgroundColor, settings.optionSelectedBackgroundColor);
        ImColorToFloat(Ui::Instance()->colors.optionSelected, settings.optionSelected);
        ImColorToFloat(Ui::Instance()->colors.optionUnselected, settings.optionUnselected);
        ImColorToFloat(Ui::Instance()->colors.optionSelectedText, settings.optionSelectedText);
        ImColorToFloat(Ui::Instance()->colors.optionUnselectedText, settings.optionUnselectedText);
        ImColorToFloat(Ui::Instance()->colors.optionBreakText, settings.optionBreakText);

        ImColorToFloat(Ui::Instance()->colors.descriptionTextColor, settings.descriptionTextColor);
        ImColorToFloat(Ui::Instance()->colors.descriptionBackgroundColor, settings.descriptionBackgroundColor);
        ImColorToFloat(Ui::Instance()->colors.descriptionSecondaryColor, settings.descriptionSecondaryColor);

        ImColorToFloat(Ui::Instance()->colors.notificationTextColor, settings.notificationTextColor);
        ImColorToFloat(Ui::Instance()->colors.notificationBackgroundColor, settings.notificationBackgroundColor);
        ImColorToFloat(Ui::Instance()->colors.notificationBarColor, settings.notificationBarColor);
    }
public:
    self_t self;
    settings_t settings;
};

#endif