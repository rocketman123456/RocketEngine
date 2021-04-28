#pragma once

#define Interface class
#define implements public
#define inheritance public

#define RK_EXPAND_MACRO(x) x
#define RK_STRINGIFY_MACRO(x) #x

#define RK_BIT(x) (1 << x)

#ifndef RK_ALIGN
#define RK_ALIGN(x, a) (((x) + ((a)-1)) & ~((a)-1))
#endif

#ifdef RK_DEBUG
    #if defined(RK_WINDOWS)
        #define RK_DEBUGBREAK() __debugbreak()
    #elif defined(RK_LINUX) || defined(RK_MACOS)
        #include <signal.h>
        #define RK_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Platform doesn't support debugbreak yet!"
    #endif
    #define RK_ENABLE_ASSERTS
#else
    #define RK_DEBUGBREAK()
#endif