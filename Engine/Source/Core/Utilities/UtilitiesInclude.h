#ifndef UTILITIES_INCLUDE_H
#define UTILITIES_INCLUDE_H

#define USING_WINDOWS ((defined(WIN32) && WIN32) || (defined(_WIN64) && _WIN64))

#if 1
#include <Windows.h>
#endif

#include <utility>
using std::move;

#pragma warning(disable : 4127) // Conditional Expression is Constant
#pragma warning(disable : 4512) // Assignment Operator Was Implicitly Defined As Deleted
#pragma warning(disable : 4010) // Single-Line Comment Contains Line-Continuation Character

#include "GlobalDefines.h"
#include "HashString.h"
#include "Debugging/DebugHelp.h"

#endif //UTILITIES_INCLUDE_H