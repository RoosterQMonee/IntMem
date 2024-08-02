#pragma once

#include <cstdint>

namespace IntMem {
	namespace Virtuals {
		template <typename Type>
		uintptr_t** GetVTable(Type* object) {
			if constexpr (!std::is_pointer_v<Type>)
				throw std::runtime_error("Type must be a pointer.");
			return reinterpret_cast<uintptr_t**>(&object);
		}

        template<int IOff, typename TRet, typename... TArgs>
        static inline TRet CallVFunc(void* vTable, TArgs... args) {
            using Fn = TRet(__fastcall*)(void*, TArgs...);
            Fn* vtable = *static_cast<Fn**>(vTable);
            return vtable[IOff](vTable, args...);
        }

        template<unsigned int IIdx, typename TRet, typename... TArgs>
        static inline auto CallVFunc2(void* thisptr, TArgs... argList) -> TRet {
            using Fn = TRet(__thiscall*)(void*, decltype(argList)...);
            return (*static_cast<Fn**>(thisptr))[IIdx](thisptr, std::forward<TArgs>(argList)...);
        }

        template <typename TRet, typename... TArgs>
        static auto CallVFuncI(uint32_t index, void* thisptr, TArgs... argList) -> TRet {
            using Fn = TRet(__thiscall*)(void*, TArgs...);
            return (*static_cast<Fn**>(thisptr))[index](thisptr, std::forward<TArgs>(argList)...);
        }

        uintptr_t AddressFromLeaInstruction(uintptr_t leaInstructionAddress)
        {
            int32_t relativeOffset = *reinterpret_cast<int32_t*>(leaInstructionAddress + 3);
            uintptr_t nextInstruction = leaInstructionAddress + 7;
            return relativeOffset + nextInstruction;
        }
	}
}