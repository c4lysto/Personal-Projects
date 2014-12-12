#ifndef DEBUGHELP_H
#define DEBUGHELP_H

#define DEBUG (_DEBUG || DEBUG_HELP_OVERRIDE)

#if DEBUG
#define DEBUG_ONLY(...) __VA_ARGS__
#else
#define DEBUG_ONLY(...)
#endif

#if DEBUG
#include <assert.h>
#endif

#if DEBUG
	#define DEBUG_TRACK_MEMORY_LEAKS(allocNum) \
	do { \
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); \
		_CrtSetBreakAlloc(allocNum); \
	} while(0)
#else
	#define DEBUG_TRACK_MEMORY_LEAKS(allocNum)
#endif

#ifndef DisplayDebugString
#if DEBUG
	#define DisplayDebugString(x, ...) \
		char buff[256]; \
		sprintf_s(buff, 256, x, __VA_ARGS__); \
		OutputDebugStringA(buff);
#else // !DEBUG
	#define DisplayDebugString(x, ...)
#endif // !DEBUG
#endif // #ifndef DisplayDebugString

#ifndef Assert
#if DEBUG
	#define __ASSERT 1
	#define Assert(condition, message, ...) \
		(void) ((!!(condition)) || \
                (1 != _CrtDbgReportW(_CRT_ASSERT, _CRT_WIDE(__FILE__), __LINE__, NULL, _CRT_WIDE(message), __VA_ARGS__)) || \
				(__debugbreak(), 0))
#else // !DEBUG
	#define __ASSERT 0
	#define Assert(condition, message, ...)
#endif // !DEBUG
#endif // ifndef Assert

#if __ASSERT
#define ASSERT_ONLY(x) x
#else // !__ASSERT
#define ASSERT_ONLY(x)
#endif // !__ASSERT

#ifndef Verify
#if DEBUG
#define Verify(condition, message, ...) \
	((condition) || \
	(1 != _CrtDbgReportW(_CRT_ASSERT, _CRT_WIDE(__FILE__), __LINE__, NULL, _CRT_WIDE(message), __VA_ARGS__)) || \
	(__debugbreak(), 0))
#else // !DEBUG
#define Verify(condition, message, ...) condition
#endif // !DEBUG
#endif // ifndef Verify

#ifndef CompileTimeAssert
#define CompileTimeAssert(contition, message) static_assert(!!(contition), message)
#endif // ifndef CompileTimeAssert

#endif // DEBUGHELP_H