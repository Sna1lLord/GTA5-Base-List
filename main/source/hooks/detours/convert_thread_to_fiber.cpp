#include "../hooks.hpp"

LPVOID Hooks::ConvertThreadToFiber(LPVOID param) {
    if (IsThreadAFiber())
		  return GetCurrentFiber();

    return static_cast<decltype(&ConvertThreadToFiber)>(Hooks::Instance()->originalConvertThreadToFiber)(param);
}