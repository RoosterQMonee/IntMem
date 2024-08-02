#pragma once

#include <IntMem/LibraryDefinitions.hpp>


#include <IntMem/Console/Logging.hpp>
#include <IntMem/Console/Exceptions.hpp>

#include <IntMem/Memory/Macros.hpp>
#include <IntMem/Memory/Process.hpp>
#include <IntMem/Memory/Access.hpp>

#ifdef INCLUDE_SCANNING
	#include <IntMem/Memory/Scanning/Signatures.hpp>
#endif

#ifdef INCLUDE_HOOKING
	#include <IntMem/Memory/Hooking/Hooking.hpp>
#endif