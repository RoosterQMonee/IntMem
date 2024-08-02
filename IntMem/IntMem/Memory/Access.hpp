#pragma once

#include <concepts>
#include <IntMem/Memory/Traits.hpp>

namespace IntMem {
    template<typename MemberType, typename Base>
    auto& MemberAt(Base* ptr, std::integral auto offset) {
        using Ret = IntMem::Traits::constness_as_t<MemberType, Base>;
        return *reinterpret_cast<Ret*>(reinterpret_cast<uintptr_t>(ptr) + offset);
    }

    template<typename Ret, typename Type>
    Ret& DirectAccess(Type* type, size_t offset) {
        if constexpr (!std::is_pointer_v<Type>)
            throw std::runtime_error("Type must be a pointer.");

        union {
            size_t raw;
            Type* source;
            Ret* target;
        } u;

        u.source = type;
        u.raw += offset;

        return *u.target;
    }
}