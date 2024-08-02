#pragma once

#include <windows.h>
#include <optional>
#include <utility>
#include <string>
#include <memory>
#include <span>
#include <bit>

namespace IntMem {
    namespace Process {
        enum class module_t : uintptr_t {};

        template<typename T> requires std::is_same_v<T, std::optional<size_t>>
        static bool isValidModule(const void* mod, const T size) {
            if (!size || *size < sizeof(IMAGE_DOS_HEADER))
                return false;

            const auto dosHeader = static_cast<const IMAGE_DOS_HEADER*>(mod);
            if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE)
                return false;

            if (*size < dosHeader->e_lfanew + sizeof(IMAGE_NT_HEADERS))
                return false;

            const auto ntHeaders = reinterpret_cast<const IMAGE_NT_HEADERS*>(static_cast<const std::byte*>(mod) + dosHeader->e_lfanew);
            if (ntHeaders->Signature != IMAGE_NT_SIGNATURE)
                return false;

            return true;
        }

        static const IMAGE_NT_HEADERS& getNTHeaders(const module_t mod) {
            const auto* scanBytes = reinterpret_cast<const std::byte*>(mod);
            const auto* dosHeader = reinterpret_cast<const IMAGE_DOS_HEADER*>(mod);
            return *reinterpret_cast<const IMAGE_NT_HEADERS*>(scanBytes + dosHeader->e_lfanew);
        }

        module_t GetProcessModule() {
            return module_t{ reinterpret_cast<uintptr_t>(GetModuleHandleA(nullptr)) };
        }

        module_t GetModule(const std::string& name) {
            const auto module = GetModuleHandleA(name.c_str());
            if (module)
                return module_t{ std::bit_cast<uintptr_t>(module) };
            return {};
        }

        std::optional<module_t> ModuleAt(void* address, std::optional<size_t> size) {
            if (address && isValidModule(address, size))
                return module_t{ std::bit_cast<uintptr_t>(address) };
            return {};
        }

        std::span<std::byte> GetModuleData(const module_t mod) {
            auto* const scanBytes = reinterpret_cast<std::byte*>(mod);
            const size_t sizeOfImage = getNTHeaders(mod).OptionalHeader.SizeOfImage;
            return { scanBytes, sizeOfImage };
        }

        std::span<std::byte> GetSectionData(const module_t mod, const std::string_view name) {
            auto* bytes = reinterpret_cast<std::byte*>(mod);
            const auto& ntHeaders = getNTHeaders(mod);
            const auto maxChars = std::min<size_t>(name.size(), 8);
            const auto* sectionHeader = IMAGE_FIRST_SECTION(&ntHeaders);

            for (int i = 0; i < ntHeaders.FileHeader.NumberOfSections; i++, sectionHeader++)
                if (strncmp(name.data(), reinterpret_cast<const char*>(sectionHeader->Name), maxChars) == 0)
                    return {
                        bytes + sectionHeader->VirtualAddress,
                        static_cast<size_t>(sectionHeader->Misc.VirtualSize)
                    };

            return {};
        }
    }
}