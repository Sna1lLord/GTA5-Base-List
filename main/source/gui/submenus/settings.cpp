#include "../ui/ui.hpp"
#include "../textures/textures.hpp"
#include "../utility/rage/invoker/natives/natives.hpp"
#include "../utility/config/config.hpp"

void Gui::Settings() {
    Ui::Instance()->AddSubmenu("Settings", [](Submenu* submenu) {
        submenu->AddOption<SubOption>("Config", "Load and save the menu config", "Config");
        submenu->AddOption<SubOption>("Header", "Change the menu header", "Header");
        submenu->AddOption<SubOption>("Colours", "Customise the menu colours", "Colours");
        submenu->AddOption<BoolOption>("Sound", "Use sounds in the menu", &Ui::Instance()->config.sound);
        submenu->AddOption<BoolOption>("Draw Bounding", "Draw bounding around menu", &Ui::Instance()->config.drawBounding);

        submenu->AddOption<NumberOption<int>>("Options Per Page", "Number of options per page", &Ui::Instance()->config.pageSize, 1, 20, 1, []{
            Ui::Instance()->config.pageSize = Ui::Instance()->config.pageSize;
        });
        submenu->AddOption<NumberOption<float>>("Rounding", "Change the menu rounding", &Ui::Instance()->config.menuRounding, 0.f, 20.f);
        submenu->AddOption<NumberOption<float>>("Position X", "Change the menu position", &Ui::Instance()->config.positionX, 0.f, 1350.f, 10.f, []{
            Ui::Instance()->config.positionX = Ui::Instance()->config.positionX;
        });
        submenu->AddOption<NumberOption<float>>("Position Y", "Change the menu position", &Ui::Instance()->config.positionY, 0.f, 1000.f, 10.f, []{
            Ui::Instance()->config.positionY = Ui::Instance()->config.positionY;
        });
        submenu->AddOption<NumberOption<float>>("Menu Width", "Change the menu width", &Ui::Instance()->config.width, 200.f, 800.f);
        submenu->AddOption<NumberOption<float>>("Bounding Thickness", "Change the menu bounding thickness", &Ui::Instance()->config.boundingThickness, 1.f, 10.f);
        submenu->AddOption<NumberOption<float>>("Option Height", "Change the option height", &Ui::Instance()->config.optionHeight, 5.f, 100.f);
        submenu->AddOption<NumberOption<float>>("Header Height", "Change the header height", &Ui::Instance()->config.headerHeight, 50.f, 300.f);
        submenu->AddOption<NumberOption<float>>("Bar Height", "Change the bar height", &Ui::Instance()->config.barHeight, 10.f, 100.f);
    });

    Ui::Instance()->AddSubmenu("Header", [](Submenu* submenu) {
        std::string path = std::format("{}\\Breeze\\Images", std::getenv("appdata"));
        
        for (auto&& entry : std::filesystem::directory_iterator(path)) {
            if (entry.exists()) {
                if (!entry.path().has_filename())
                    continue;

                submenu->AddOption<RegularOption>(entry.path().filename().string().c_str(), "Load the selected header texture", [=]{
                    Texture(
                        entry.path().string().c_str(), 
                        &Paint::Instance()->texture, 
                        &Ui::Instance()->config.width,
                        &Ui::Instance()->config.headerHeight
                    );
                });
            }
        }

        submenu->AddOption<RegularOption>("Default", "Load the selected header texture", [=]{
            Paint::Instance()->texture = nullptr;
        });
    });

    Ui::Instance()->AddSubmenu("Colours", [](Submenu* submenu) {
        submenu->AddOption<PaletteOption>("Bounding", "Change the menu bounding color", Config::Instance()->settings.boundingColor, [=]{
            Ui::Instance()->colors.boundingColor = Config::Instance()->FloatToImColor(Config::Instance()->settings.boundingColor);
        });
        submenu->AddOption<PaletteOption>("Header Background", "Change the header background color", Config::Instance()->settings.headerBackgroundColor, [=]{
            Ui::Instance()->colors.headerBackgroundColor = Config::Instance()->FloatToImColor(Config::Instance()->settings.headerBackgroundColor);
        });

        submenu->AddOption<PaletteOption>("Header Text", "Change the header text color", Config::Instance()->settings.headerTextColor, [=]{
            Ui::Instance()->colors.headerTextColor = Config::Instance()->FloatToImColor(Config::Instance()->settings.headerTextColor);
        });

        submenu->AddOption<PaletteOption>("Small Bar Background", "Change the small bar background color", Config::Instance()->settings.smallBarBackgroundColor, [=]{
            Ui::Instance()->colors.smallBarBackgroundColor = Config::Instance()->FloatToImColor(Config::Instance()->settings.smallBarBackgroundColor);
        });

        submenu->AddOption<PaletteOption>("Bar Background", "Change the bar background color", Config::Instance()->settings.barBackgroundColor, [=]{
            Ui::Instance()->colors.barBackgroundColor = Config::Instance()->FloatToImColor(Config::Instance()->settings.barBackgroundColor);
        });

        submenu->AddOption<PaletteOption>("Bar Text", "Change the bar text color", Config::Instance()->settings.barTextColor, [=]{
            Ui::Instance()->colors.barTextColor = Config::Instance()->FloatToImColor(Config::Instance()->settings.barTextColor);
        });

        submenu->AddOption<PaletteOption>("Option Selected Background", "Change the selected option background color", Config::Instance()->settings.optionSelectedBackgroundColor, [=]{
            Ui::Instance()->colors.optionSelectedBackgroundColor = Config::Instance()->FloatToImColor(Config::Instance()->settings.optionSelectedBackgroundColor);
        });

        submenu->AddOption<PaletteOption>("Option Selected", "Change the selected option color", Config::Instance()->settings.optionSelected, [=]{
            Ui::Instance()->colors.optionSelected = Config::Instance()->FloatToImColor(Config::Instance()->settings.optionSelected);
        });

        submenu->AddOption<PaletteOption>("Option Unselected", "Change the unselected option color", Config::Instance()->settings.optionUnselected, [=]{
            Ui::Instance()->colors.optionUnselected = Config::Instance()->FloatToImColor(Config::Instance()->settings.optionUnselected);
        });

        submenu->AddOption<PaletteOption>("Option Selected Text", "Change the text color of selected options", Config::Instance()->settings.optionSelectedText, [=]{
            Ui::Instance()->colors.optionSelectedText = Config::Instance()->FloatToImColor(Config::Instance()->settings.optionSelectedText);
        });

        submenu->AddOption<PaletteOption>("Option Unselected Text", "Change the text color of unselected options", Config::Instance()->settings.optionUnselectedText, [=]{
            Ui::Instance()->colors.optionUnselectedText = Config::Instance()->FloatToImColor(Config::Instance()->settings.optionUnselectedText);
        });

        submenu->AddOption<PaletteOption>("Option Break Text", "Change the text color of option breaks", Config::Instance()->settings.optionBreakText, [=]{
            Ui::Instance()->colors.optionBreakText = Config::Instance()->FloatToImColor(Config::Instance()->settings.optionBreakText);
        });

        submenu->AddOption<PaletteOption>("Description Text", "Change the text color of descriptions", Config::Instance()->settings.descriptionTextColor, [=]{
            Ui::Instance()->colors.descriptionTextColor = Config::Instance()->FloatToImColor(Config::Instance()->settings.descriptionTextColor);
        });

        submenu->AddOption<PaletteOption>("Description Background", "Change the background color of descriptions", Config::Instance()->settings.descriptionBackgroundColor, [=]{
            Ui::Instance()->colors.descriptionBackgroundColor = Config::Instance()->FloatToImColor(Config::Instance()->settings.descriptionBackgroundColor);
        });

        submenu->AddOption<PaletteOption>("Description Secondary", "Change the secondary color of descriptions", Config::Instance()->settings.descriptionSecondaryColor, [=]{
            Ui::Instance()->colors.descriptionSecondaryColor = Config::Instance()->FloatToImColor(Config::Instance()->settings.descriptionSecondaryColor);
        });

        submenu->AddOption<PaletteOption>("Notification Text", "Change the text color of notifications", Config::Instance()->settings.notificationTextColor, [=]{
            Ui::Instance()->colors.notificationTextColor = Config::Instance()->FloatToImColor(Config::Instance()->settings.notificationTextColor);
        });

        submenu->AddOption<PaletteOption>("Notification Background", "Change the background color of notifications", Config::Instance()->settings.notificationBackgroundColor, [=]{
            Ui::Instance()->colors.notificationBackgroundColor = Config::Instance()->FloatToImColor(Config::Instance()->settings.notificationBackgroundColor);
        });

        submenu->AddOption<PaletteOption>("Notification Bar", "Change the color of notification bars", Config::Instance()->settings.notificationBarColor, [=]{
            Ui::Instance()->colors.notificationBarColor = Config::Instance()->FloatToImColor(Config::Instance()->settings.notificationBarColor);
        });
    });

    Ui::Instance()->AddSubmenu("Config", [](Submenu* submenu) {
        submenu->AddOption<InputOption>("Save", "Save the current config", Config::Instance()->settings.filename, 15, []{
            Config::Instance()->Save(Config::Instance()->settings.filename);
        });
        
        std::string path = std::format("{}\\Breeze\\Config", std::getenv("appdata"));
        for (auto&& entry : std::filesystem::directory_iterator(path)) {
            if (entry.exists()) {
                if (!entry.path().has_filename())
                    continue;

                submenu->AddOption<RegularOption>(entry.path().filename().string().c_str(), "Load the selected config", [=]{
                    Config::Instance()->Load(entry.path().filename().string().c_str());
                });
            }
        }
    });
}