#ifndef _FEATURES_HPP
#define _FEATURES_HPP

#include "../utility/rage/rage.hpp"

class Features {
public:
    static Features* Instance();
    void Initialize();
    void Uninitialize();
    void Tick();
public:
    int localPed;
    int localPlayer;
    CPedFactory* pedFactory;
    CPed* localPedPointer;
    CPlayerInfo* localPlayerInfo;
};

#endif