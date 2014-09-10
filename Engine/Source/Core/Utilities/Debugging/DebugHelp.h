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

#ifndef DisplayDebugString
#if DEBUG
	#define DisplayDebugString(x, ...) \
		char buff[256]; \
		sprintf(buff, x, __VA_ARGS__); \
		OutputDebugString(buff);
#else // !DEBUG
	#define DisplayDebugString(x, ...)
#endif // !DEBUG
#endif // #ifndef DisplayDebugString

#ifndef Assert
#if DEBUG
	#define Assert(condition, message, ...) \
		(void) ((!!(condition)) || \
                (1 != _CrtDbgReportW(_CRT_ASSERT, _CRT_WIDE(__FILE__), __LINE__, NULL, _CRT_WIDE(message), __VA_ARGS__)) || \
				(__debugbreak(), 0))
#define ASSERT_ONLY(x) x
#else // !DEBUG
	#define Assert(condition, message, ...)
	#define ASSERT_ONLY(x)
#endif // !DEBUG
#endif // ifndef Assert

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
#if DEBUG
#define CompileTimeAssert(contition) static_assert(!!(contition), "Compile Error!!!")
#else // !DEBUG
	#define CompileTimeAssert(condition)
#endif // !DEBUG
#endif // ifndef CompileTimeAssert

#endif // DEBUGHELP_H