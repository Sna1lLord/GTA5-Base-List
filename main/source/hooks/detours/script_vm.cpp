#include "../hooks.hpp"
#include "../utility/logger/logger.hpp"
#include "../utility/signatures/signatures.hpp"
#include "../utility/rage/invoker/queue.hpp"
#include "../utility/rage/invoker/invoker.hpp"
#include "../utility/rage/invoker/natives/natives.hpp"
#include "../source/script/manager.hpp"

static uint32_t thisFrameCount;

__int64 Hooks::ScriptVm(__int64 a1, __int64** a2, unsigned __int64* a3, __int64 a4) {
    uint32_t frameCount = *Signatures::Instance()->definitions.frameCount;

    if (thisFrameCount != frameCount) {
        invoker.Cache();
        
        ScriptManager::Instance()->Tick();

        thisFrameCount = frameCount;
    }

    return static_cast<decltype(&ScriptVm)>(Hooks::Instance()->originalScriptVm)(a1, a2, a3, a4);
}