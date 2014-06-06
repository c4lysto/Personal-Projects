#ifndef DEBUGHELP_H
#define DEBUGHELP_H

#define DEBUG_HELP (_DEBUG || DEBUG_HELP_OVERRIDE)

#if DEBUG_HELP
#include <assert.h>
#endif


#if DEBUG_HELP
	#define Assert(condition, message, ...) \
		(void) ((!!(condition)) || \
                (1 != _CrtDbgReportW(_CRT_ASSERT, _CRT_WIDE(__FILE__), __LINE__, NULL, _CRT_WIDE(message), __VA_ARGS__)) || \
				(__debugbreak(), 0))
#else
	#define Assert(condition, message, ...) ((void)0)
#endif

#endif //DEBUGHELP_H