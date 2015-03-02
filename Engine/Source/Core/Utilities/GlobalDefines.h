#ifndef GLOBAL_DEFINES_H
#define GLOBAL_DEFINES_H

typedef __int8 s8;
typedef unsigned __int8 u8;
typedef __int16 s16;
typedef unsigned __int16 u16;
typedef __int32 s32;
typedef unsigned __int32 u32;
typedef __int64 s64;
typedef unsigned __int64 u64;

#ifndef SSE_AVAILABLE
#if defined(_M_IX86) || defined(_M_AMD64)
#define SSE_AVAILABLE 1
#else
#define SSE_AVAILABLE 0
#endif
#endif // SSE_AVAILABLE

#ifndef ALIGN
#define ALIGN(byteAlign) __declspec(align(byteAlign))
#endif

#ifndef GLOBALCONST
#define GLOBALCONST extern const __declspec(selectany)
#endif

#ifndef THREADLOCAL
#define THREADLOCAL __declspec(thread)
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(rel) { if(rel) {rel->Release(); rel = nullptr;}}
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(del) { if(del) {delete del; del = nullptr;} }
#endif

#ifndef NUMELEM
#define NUMELEM(arr) (sizeof(arr)/(*(arr)))
#endif

#ifndef TO_STRING
#define TO_STRING(str) #str
#endif

#define ONE_OVER_255 (1.0f/255.0f)

#define CHAR_MASK (0xFFL)
#define SHORT_MASK (0xFFFFL)
#define INT_MASK (0xFFFFFFFFL)
#define LONG_MASK INT_MASK
#define LONG_LONG_MASK (0xFFFFFFFFFFFFFFFFLL)

#ifndef CHAR_MIN
#define CHAR_MIN	(-128)
#endif // CHAR_MIN

#ifndef S8_MIN
#define S8_MIN		CHAR_MIN
#endif // S8_MIN

#ifndef CHAR_MAX
#define CHAR_MAX	(127)
#endif // CHAR_MAX

#ifndef S8_MAX
#define S8_MAX		CHAR_MAX
#endif // S8_MAX

#ifndef UCHAR_MAX
#define UCHAR_MAX	(0xFF)
#endif // UCHAR_MAX

#ifndef U8_MAX
#define U8_MAX		UCHAR_MAX
#endif // U8_MAX

#ifndef SHORT_MIN
#define SHORT_MIN	(-32768)
#endif // SHORT_MIN

#ifndef S16_MIN
#define S16_MIN		SHORT_MIN
#endif // S8_MIN

#ifndef SHORT_MAX
#define SHORT_MAX	(32767)
#endif // SHORT_MAX

#ifndef S16_MAX
#define S16_MAX		SHORT_MAX
#endif // S8_MAX

#ifndef USHORT_MAX
#define USHORT_MAX	(0xFFFF)
#endif // USHORT_MAX

#ifndef U16_MAX
#define U16_MAX		USHORT_MAX
#endif // S8_MAX

#ifndef INT_MIN
#define INT_MIN		(-2147483648)
#endif // INT_MIN

#ifndef LONG_MIN
#define LONG_MIN	INT_MIN
#endif // LONG_MIN

#ifndef S32_MIN
#define S32_MIN		INT_MIN
#endif // S32_MIN

#ifndef INT_MAX
#define	INT_MAX		(2147483647)
#endif // INT_MAX

#ifndef LONG_MAX
#define LONG_MAX	INT_MAX
#endif // LONG_MAX

#ifndef S32_MAX
#define S32_MAX		INT_MAX
#endif // S32_MAX

#ifndef UINT_MAX
#define	UINT_MAX	(0xFFFFFFFF)
#endif // UINT_MAX

#ifndef ULONG_MAX
#define ULONG_MAX	UINT_MAX
#endif // ULONG_MAX

#ifndef U32_MAX
#define U32_MAX		UINT_MAX
#endif // U32_MAX

#ifndef LLONG_MIN
#define LLONG_MIN	(9223372036854775807i64)
#endif // LLONG_MIN

#ifndef S64_MIN
#define S64_MIN		LLONG_MIN
#endif // S64_MIN

#ifndef LLONG_MAX
#define LLONG_MAX	(9223372036854775808i64)
#endif // LLONG_MAX

#ifndef S64_MAX
#define S64_MAX		LLONG_MAX
#endif // S64_MAX

#ifndef ULLONG_MAX
#define ULLONG_MAX	(0xFFFFFFFFFFFFFFFFui64)
#endif // ULLONG_MAX

#ifndef U64_MAX
#define U64_MAX		ULLONG_MAX
#endif // U64_MAX

#define BIT(bit) (1<<bit)
#define BIT0 BIT(0)
#define BIT1 BIT(1)
#define BIT2 BIT(2)
#define BIT3 BIT(3)
#define BIT4 BIT(4)
#define BIT5 BIT(5)
#define BIT6 BIT(6)
#define BIT7 BIT(7)
#define BIT8 BIT(8)
#define BIT9 BIT(9)
#define BIT10 BIT(10)
#define BIT11 BIT(11)
#define BIT12 BIT(12)
#define BIT13 BIT(13)
#define BIT14 BIT(14)
#define BIT15 BIT(15)
#define BIT16 BIT(16)
#define BIT17 BIT(17)
#define BIT18 BIT(18)
#define BIT19 BIT(19)
#define BIT20 BIT(20)
#define BIT21 BIT(21)
#define BIT22 BIT(22)
#define BIT23 BIT(23)
#define BIT24 BIT(24)
#define BIT25 BIT(25)
#define BIT26 BIT(26)
#define BIT27 BIT(27)
#define BIT28 BIT(28)
#define BIT29 BIT(29)
#define BIT30 BIT(30)
#define BIT31 BIT(31)

#endif