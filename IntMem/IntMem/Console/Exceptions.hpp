#pragma once
#include <string>


template <typename... T>
[[noreturn]] inline void _Assert(const char* function, int line, std::string formatted_string)
{
    throw std::exception((std::string(function) + ":" + std::to_string(line) + "\n" + formatted_string).c_str());
}

#define Assert(...) Log::_Assert(__FUNCTION__, __LINE__, __VA_ARGS__)
