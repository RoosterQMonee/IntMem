#pragma once

#include <IntMem/Memory/Access.hpp>


#define INRANGE(x, a, b) (x >= a && x <= b)
#define GET_BITS(x) (INRANGE((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xa) : (INRANGE(x, '0', '9') ? (x - '0') : 0))
#define GET_BYTE(x) ((GET_BITS(x[0]) << 4) | GET_BITS(x[1]))

#define AS_FIELD(type, name, fn) __declspec(property(get = fn, put = set##name)) type name
#define DEF_FIELD_RW(type, name) __declspec(property(get = get##name, put = set##name)) type name
#define FAKE_FIELD(type, name) AS_FIELD(type, name, get##name); type get##name()

#define BUILD_ACCESS(ptr, type, name, offset)                                                                        \
AS_FIELD(type, name, get##name);                                                                                     \
type get##name() const { if (ptr) return IntMem::DirectAccess<type>(ptr, offset); else return (type)0; }                                                     \
void set##name(type v) const {if (ptr) IntMem::DirectAccess<type>(ptr, offset) = v; }

#define BUILD_ACCESS_STRING(ptr, name, offset) \
AS_FIELD(std::string, name, get##name);        \
std::string get##name() const { if (ptr) return IntMem::DirectAccess<std::string>(ptr, offset); else return ""; } \
void set##name(std::string v) const {if (ptr) IntMem::DirectAccess<std::string>(ptr, offset) = v; }
