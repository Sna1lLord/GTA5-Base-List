#include "../hooks.hpp"
#include "../source/gui/gui.hpp"
#include "../utility/logger/logger.hpp"

void Hooks::OnPlayerJoin(__int64 a1, CNetGamePlayer* a2) {
    //Gui::Instance()->AddNotification("%s Joined", a2->get_name());

    if (a2->get_net_data()->m_host_token < 200'000'000) {
        //Gui::Instance()->AddNotification("%s Triggered Detection", a2->get_name());
    }

    return static_cast<decltype(&OnPlayerJoin)>(Hooks::Instance()->originalOnPlayerJoin)(a1, a2);
}

void Hooks::OnPlayerLeave(__int64 a1, CNetGamePlayer* a2) {
    //Gui::Instance()->AddNotification("%s Left", a2->get_name());

    return static_cast<decltype(&OnPlayerLeave)>(Hooks::Instance()->originalOnPlayerLeave)(a1, a2);
}