#pragma once

#include <concepts>
#include <IntMem/Memory/Scanning/Result.hpp>
#include <iterator>
#include <string_view>


namespace IntMem {
    template<typename Fn, typename Ret>
    concept supplier = requires(Fn && fn) {
        { fn() } -> std::same_as<Ret>;
    };

    template<typename Fn>
    concept function = std::is_function_v<Fn>;

    template<typename From, typename To>
    concept reinterpret_as = sizeof(To) == sizeof(From)
        && std::is_trivially_copyable_v<From>
        && std::is_trivially_copyable_v<To>
        && std::is_trivially_constructible_v<To>;

    template<typename T>
    concept byte_input_iterator = std::input_iterator<T>
        && std::forward_iterator<T>
        && std::contiguous_iterator<T>
        && std::same_as<std::iter_value_t<T>, std::byte>;

    template<typename T>
    concept char_iterator = std::is_same_v<std::iter_value_t<T>, char>;

    enum class parse_int_error {
        invalid_base,
        illegal_char
    };

    template<std::integral Integer, char_iterator Iter>
    inline constexpr IntMem::result<Integer, parse_int_error> parse_int(Iter begin, Iter end, int base = 10) noexcept {
        if (base < 2 || base > 36)
            return result_error{ parse_int_error::invalid_base };

        Integer sign = 1;
        Integer value = 0;
        const auto digits = base < 10 ? base : 10;
        const auto letters = base > 10 ? base - 10 : 0;

        for (auto iter = begin; iter != end; iter++) {
            const char ch = *iter;

            if constexpr (std::is_signed_v<Integer>) {
                if (iter == begin) {
                    if (ch == '+')
                        continue;

                    else if (ch == '-') {
                        sign = -1;
                        continue;
                    }
                }
            }

            value *= base;
            if (ch >= '0' && ch < '0' + digits)
                value += (ch - '0');
            else if (ch >= 'A' && ch < 'A' + letters)
                value += (ch - 'A' + 10);
            else if (ch >= 'a' && ch < 'a' + letters)
                value += (ch - 'a' + 10);
            else
                return result_error{ parse_int_error::illegal_char };
        }
        return sign * value;
    }

    template<typename Integer>
    inline constexpr result<Integer, parse_int_error> parse_int(std::string_view str, int base = 10) noexcept {
        return parse_int<Integer>(str.cbegin(), str.cend(), base);
    }
}