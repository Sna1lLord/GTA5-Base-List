#ifndef _HOOKS_HPP
#define _HOOKS_HPP

#include <common.hpp>
#include "../utility/rage/rage.hpp"

class VTSwap {
public:
    static VTSwap* Instance();
public:
    void Create(void* address, int index, void* detour, void** original, const char* name);
    void Destroy();
public:
    std::vector<std::tuple<void*, int, void**>> swapped;
};

class Hooks {
private:
    void Detour(uintptr_t address, void* detour, void** original, const char* name);
public:
    static Hooks* Instance();
    void Initialize();
    void Uninitialize();
public:
    WNDPROC originalWndProc;
    void* originalPresent;
    static HRESULT __stdcall Present(
        IDXGISwapChain* swapChain,
        UINT syncInterval, 
        UINT flags
    );

    void* originalScriptVm;
    static __int64 ScriptVm(
        __int64 a1, 
        __int64** a2, 
        unsigned __int64* a3, 
        __int64 a4
    );

    void* originalConvertThreadToFiber;
    static LPVOID ConvertThreadToFiber(
        LPVOID param
    );

    void* originalOnPlayerJoin;
    static void OnPlayerJoin(
        __int64 a1,
        CNetGamePlayer* a2
    );

    void* originalOnPlayerLeave;
    static void OnPlayerLeave(
        __int64 a1,
        CNetGamePlayer* a2
    );
};

#endif