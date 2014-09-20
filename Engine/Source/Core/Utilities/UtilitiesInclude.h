#ifndef UTILITIES_INCLUDE_H
#define UTILITIES_INCLUDE_H

#if WIN32 || _WIN64
#include <Windows.h>
#endif

#include <utility>
using std::move;

#pragma warning(disable : 4127) // Conditional Expression is Constant

#include "GlobalDefines.h"
#include "Debugging/DebugHelp.h"

#endif //UTILITIES_INCLUDE_H