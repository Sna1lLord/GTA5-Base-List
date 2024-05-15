#ifndef _INVOKER_HPP
#define _INVOKER_HPP

#include <common.hpp>
#include "../rage.hpp"

class NativeCallContext : public scrNativeCallContext {
public:
	NativeCallContext();
private:
	std::uint64_t returnStack[10];
	std::uint64_t argumentStack[100];
};

class Invoker {
public:
    void Cache();
    void Begin();
    void End(scrNativeHash hash);

    template <typename T>
    void Push(T&& value) {
        callContext.Push(std::forward<T>(value));
    }

    template <typename T>
    T& Get() {
        return *callContext.GetReturn<T>();
    }
private:
    bool handlersCached = false;
    NativeCallContext callContext;
    std::unordered_map<scrNativeHash, scrNativeHandler> handlerCache;
};
inline Invoker invoker;

#endif