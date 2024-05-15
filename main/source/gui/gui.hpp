#ifndef _GUI_HPP
#define _GUI_HPP

#include <common.hpp>
#include "fonts.hpp"

class Gui {
public:
    ID3D11Device* device;
	ID3D11DeviceContext* context;
    HWND hwnd;
    bool initialized;
public:
    static Gui* Instance();
    void Initialize();
    void Uninitialize();
    void Tick();
public:
    static void Self();
    static void Weapon();
    static void Vehicle();
    static void Session();
    static void Teleport();
    static void Recovery();
    static void World();
    static void Protections();
    static void Settings();
};

#endif