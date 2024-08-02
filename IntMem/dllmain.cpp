/*

    IntMem Usage Example

* Internal Minecraft: Bedrock Edition modifications
* Version: 1.21.2

*/

#include <Windows.h>
#include <IntMem/IntMem.hpp>


void Init() {
    Logger::Initialize("log.txt");
    Logger::Log<LogType::Info>("Loaded console.");

    IntMem::Process::module_t modhandle = IntMem::Process::GetModule("Minecraft.Windows.exe");
    IntMem::Signatures::fixed_signature pattern = IntMem::Signatures::compile_signature<"48 83 EC ? ? ? C1 4C 8D 05">(); // yippee
    
    Logger::Shutdown();
}

bool __stdcall DllMain(const HINSTANCE hinstDLL, const DWORD fdwReason, [[maybe_unused]] LPVOID lpReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);
        CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Init), nullptr, 0, nullptr);
    }

    return true;
}

