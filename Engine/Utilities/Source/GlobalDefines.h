#ifndef GLOBAL_DEFINES_H
#define GLOBAL_DEFINES_H

#define ALIGN(byteAlign) __declspec(align(byteAlign))

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(rel) { if(rel) {rel->Release(); rel = nullptr;}}
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(del) { if(del) {delete del; del = nullptr;} }
#endif

#endif