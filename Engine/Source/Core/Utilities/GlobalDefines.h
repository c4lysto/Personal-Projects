#ifndef GLOBAL_DEFINES_H
#define GLOBAL_DEFINES_H

typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

#ifndef SSE_AVAILABLE
#if defined(_M_IX86) || defined(_M_AMD64)
#define SSE_AVAILABLE 1
#else
#define SSE_AVAILABLE 0
#endif
#endif // SSE_AVAILABLE

#define ALIGN(byteAlign) __declspec(align(byteAlign))

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

#define CHAR_MASK (0xFFL)
#define SHORT_MASK (0xFFFFL)
#define INT_MASK (0xFFFFFFFFL)
#define LONG_MASK INT_MASK
#define LONG_LONG_MASK (0xFFFFFFFFFFFFFFFFLL)

#define BIT0 (0x1)
#define BIT1 (0x2)
#define BIT2 (0x4)
#define BIT3 (0x8)
#define BIT4 (0x10)
#define BIT5 (0x20)
#define BIT6 (0x40)
#define BIT7 (0x80)
#define BIT8 (0x100)
#define BIT9 (0x200)
#define BIT10 (0x400)
#define BIT11 (0x800)
#define BIT12 (0x1000)
#define BIT13 (0x2000)
#define BIT14 (0x4000)
#define BIT15 (0x8000)
#define BIT16 (0x10000)
#define BIT17 (0x20000)
#define BIT18 (0x40000)
#define BIT19 (0x80000)
#define BIT20 (0x100000)
#define BIT21 (0x200000)
#define BIT22 (0x400000)
#define BIT23 (0x800000)
#define BIT24 (0x1000000)
#define BIT25 (0x2000000)
#define BIT26 (0x4000000)
#define BIT27 (0x8000000)
#define BIT28 (0x10000000)
#define BIT29 (0x20000000)
#define BIT30 (0x40000000)
#define BIT31 (0x80000000)

#endif