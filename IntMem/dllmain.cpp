/*

    IntMem Usage Example

* Internal Minecraft: Bedrock Edition modifications
* Version: 1.21.2

*/

#include <Windows.h>


void Init() {

}

bool __stdcall DllMain(const HINSTANCE hinstDLL, const DWORD fdwReason, [[maybe_unused]] LPVOID lpReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);
        CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Init), nullptr, 0, nullptr);
    }

    return true;
}

