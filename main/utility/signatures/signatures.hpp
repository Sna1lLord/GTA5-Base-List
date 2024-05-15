#ifndef _SCANNER_HPP
#define _SCANNER_HPP

#include <common.hpp>
#include "../rage/rage.hpp"
#include "functions.hpp"

struct SignatureDefinitions_t {
    IDXGISwapChain** swapchain;

    uint32_t* frameCount;
    uintptr_t scriptVm;

    scrNativeRegistrationTable* nativeRegistrationTable;
    Functions::GetNativeHandlerFn getNativeHandler;
    PVOID nativeReturn;
    Functions::FixVectorsFn fixVectors;

    __int64* transactionQueue;
    Functions::ConstructBasketFn constructBasket;
    Functions::AddItemToBasketFn addItemToBasket;
    Functions::ProcessTransactionFn processTransaction;

    int* gameState;

    CPedFactory** pedFactory;

    uintptr_t onPlayerJoin;
    uintptr_t onPlayerLeave;
};

class Module {
private:
    HANDLE handle;
public:
    size_t size;
    uintptr_t base;
    uint8_t* bytes;

    Module() = default;

    Module(const char* name) {
        handle = GetCurrentProcess();
        HMODULE moduleHandle = GetModuleHandleA(name);
        MODULEINFO moduleInfo;
        K32GetModuleInformation(handle, moduleHandle, &moduleInfo, sizeof(MODULEINFO));

        size = (size_t)moduleInfo.SizeOfImage;
        base = (uintptr_t)moduleInfo.lpBaseOfDll;
        bytes = reinterpret_cast<uint8_t*>(moduleInfo.lpBaseOfDll);
    }
};

class Scanner {
public:
    uint8_t* moduleBytes;
    std::vector<uint8_t> signatureBytes;
    size_t size = 0;
    uintptr_t base = 0;

    Scanner(){};
    Scanner(const std::string signature);
};

class Result {
private:
    uintptr_t address = 0;
public:
    Result() {}
    Result(uintptr_t result);
    Result Add(uintptr_t value);
    Result Sub(uintptr_t value);
    Result Rip(uintptr_t add = 0x3);

    template <typename T>
    T Cast();
};

class Scan {
private:
    uintptr_t result = 0;
public:
    Scan(){};
    Scan(std::string signature, Module module = Module("gta5.exe"));
    Result Get();
};
inline Scan g_Scan;

class Signatures {
public:
    SignatureDefinitions_t definitions;
public:
    static Signatures* Instance();
    void Initialize();
    void Uninitialize();
};

#endif