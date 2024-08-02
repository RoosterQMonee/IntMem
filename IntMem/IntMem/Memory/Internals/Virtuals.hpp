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
	}
}