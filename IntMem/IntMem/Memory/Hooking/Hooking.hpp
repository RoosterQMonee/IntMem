#pragma once

#include <IntMem/External/MinHook/Minhook.h>


namespace IntMem {
	namespace Hooking {
        template <typename T>
        bool HookFunction(void* pTarget, T pDetour, void* pOriginal) {
            if (MH_CreateHook(pTarget, pDetour, (LPVOID*)pOriginal) != MH_OK)
                return false;

            if (MH_EnableHook(pTarget) != MH_OK)
                return false;

            return true;
        }

        template <typename R, typename... Args>
        R CallFunc(void* func, Args... args)
        {
            return ((R(*)(Args...))func)(args...);
        }
	}
}