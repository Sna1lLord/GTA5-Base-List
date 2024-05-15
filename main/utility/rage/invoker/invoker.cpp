#include "invoker.hpp"
#include "crossmap.hpp"
#include "../utility/signatures/signatures.hpp"

extern "C" void	_call_asm(void* context, void* function, void* ret);

NativeCallContext::NativeCallContext() {
	returnValue = &returnStack[0];
	arguments = &argumentStack[0];
}

void Invoker::Cache() {
    if (handlersCached)
        return;

    //std::ofstream entrypoints(std::format("{}\\Breeze\\Dev\\entrypoints.txt", std::getenv("appdata")));

    for (const scrNativeMapping& mapping : crossmap) {
        scrNativeHandler handler = Signatures::Instance()->definitions.getNativeHandler(
            Signatures::Instance()->definitions.nativeRegistrationTable, 
            mapping.second
        );

        //entrypoints << "0x" << std::hex << std::uppercase << mapping.first << " : " << std::hex << std::uppercase << (uintptr_t)handler - (uintptr_t)GetModuleHandleA("gta5.exe") << "\n";

        handlerCache.emplace(mapping.first, handler);
    }

    //entrypoints.close();

    handlersCached = true;
}

void Invoker::Begin() {
    callContext.Reset();
}

void Invoker::End(scrNativeHash hash) {
    if (auto it = handlerCache.find(hash); it != handlerCache.end()) {
        scrNativeHandler handler = it->second;

        Signatures::Instance()->definitions.fixVectors(&callContext);
        _call_asm(&callContext, handler, Signatures::Instance()->definitions.nativeReturn);
    }
}