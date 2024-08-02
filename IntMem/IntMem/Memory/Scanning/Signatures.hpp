#pragma once

#include <array>
#include <bit>
#include <optional>
#include <ranges>
#include <string_view>
#include <vector>

#include <IntMem/Memory/Internals/FixedString.hpp>
#include <IntMem/Memory/Scanning/Result.hpp>
#include <IntMem/Memory/Scanning/Parsing.hpp>


namespace IntMem {
	namespace Signatures {
        struct signature_element {
            constexpr signature_element() noexcept {}
            constexpr signature_element(std::nullopt_t) noexcept {}
            constexpr signature_element(const std::byte valueIn) noexcept : val(valueIn), present(true) {}

            constexpr signature_element& operator=(std::nullopt_t) noexcept {
                return *this = signature_element{};
            }

            constexpr signature_element& operator=(const std::byte valueIn) noexcept {
                return *this = signature_element{ valueIn };
            }

            constexpr void reset(std::nullopt_t) noexcept {
                *this = std::nullopt;
            }

            [[nodiscard]] constexpr bool has_value() const noexcept {
                return this->present;
            }

            [[nodiscard]] constexpr std::byte value() const noexcept {
                return this->val;
            }

            [[nodiscard]] constexpr operator bool() const noexcept {
                return this->has_value();
            }

            [[nodiscard]] constexpr std::byte operator*() const noexcept {
                return this->value();
            }
        private:
            std::byte val{};
            bool present = false;
        };

        using signature = std::vector<signature_element>;
        using signature_view = std::span<const signature_element>;

        template<size_t N>
        using fixed_signature = std::array<signature_element, N>;
        
        enum class signature_parse_error {
            missing_byte,
            parse_error,
            empty_signature,
        };

        [[nodiscard]] constexpr IntMem::result<signature, signature_parse_error> parse_signature(std::string_view str) {
            signature sig{};
            bool containsByte = false;
            for (const auto& word : str | std::views::split(' ')) {
                if (word.empty())
                    continue;
                if (word[0] == '?')
                    sig.emplace_back(std::nullopt);

                else {
                    const auto sv = std::string_view{ word.begin(), word.end() };
                    const auto parsed = IntMem::parse_int<uint8_t>(sv, 16);
                    if (parsed.has_value())
                        sig.emplace_back(static_cast<std::byte>(parsed.value()));
                    else
                        return result_error{ signature_parse_error::parse_error };
                    containsByte = true;
                }
            }

            if (sig.empty())
                return result_error{ signature_parse_error::empty_signature };
            if (!containsByte)
                return result_error{ signature_parse_error::missing_byte };
            return sig;
        }

        template<IntMem::fixed_string str>
        [[nodiscard]] consteval auto compile_signature() {
            const auto sig = parse_signature(str.c_str()).value();
            constexpr auto N = parse_signature(str.c_str()).value().size();
            fixed_signature<N> arr{};
            std::ranges::move(sig, arr.begin());
            return arr;
        }
	}
}