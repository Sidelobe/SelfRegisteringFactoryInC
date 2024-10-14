//   ▗▄▄▗▄▄▄▗▖  ▗▄▄▄▖    ▗▄▄▖▗▄▄▄▖▗▄▄▗▄▄▄▖▗▄▄▗▄▄▄▗▄▄▄▗▄▄▖▗▄▄▄▗▖  ▗▖▗▄▄▖                 
//  ▐▌  ▐▌  ▐▌  ▐▌       ▐▌ ▐▐▌  ▐▌    █ ▐▌    █ ▐▌  ▐▌ ▐▌ █ ▐▛▚▖▐▐▌                    
//   ▝▀▚▐▛▀▀▐▌  ▐▛▀▀▘    ▐▛▀▚▐▛▀▀▐▌▝▜▌ █  ▝▀▚▖ █ ▐▛▀▀▐▛▀▚▖ █ ▐▌ ▝▜▐▌▝▜▌                 
//  ▗▄▄▞▐▙▄▄▐▙▄▄▐▌       ▐▌ ▐▐▙▄▄▝▚▄▞▗▄█▄▗▄▄▞▘ █ ▐▙▄▄▐▌ ▐▗▄█▄▐▌  ▐▝▚▄▞▘                 
//  ▗▄▄▄▖▗▄▖ ▗▄▄▗▄▄▄▗▄▖▗▄▄▗▖  ▗▖    ▗▄▄▖ ▗▄▗▄▄▄▗▄▄▄▗▄▄▄▗▄▄▖▗▖  ▗▖    ▗▄▄▄▗▖  ▗▖     ▗▄▄▖
//  ▐▌  ▐▌ ▐▐▌    █▐▌ ▐▐▌ ▐▝▚▞▘     ▐▌ ▐▐▌ ▐▌█   █ ▐▌  ▐▌ ▐▐▛▚▖▐▌      █ ▐▛▚▖▐▌    ▐▌   
//  ▐▛▀▀▐▛▀▜▐▌    █▐▌ ▐▐▛▀▚▖▐▌      ▐▛▀▘▐▛▀▜▌█   █ ▐▛▀▀▐▛▀▚▐▌ ▝▜▌      █ ▐▌ ▝▜▌    ▐▌   
//  ▐▌  ▐▌ ▐▝▚▄▄▖ █▝▚▄▞▐▌ ▐▌▐▌      ▐▌  ▐▌ ▐▌█   █ ▐▙▄▄▐▌ ▐▐▌  ▐▌    ▗▄█▄▐▌  ▐▌    ▝▚▄▄▖
//                                                                                      
//  © 2024 - https://github.com/Sidelobe/SelfRegisteringFactoryInC

#ifndef FactoryRegistration_h
#define FactoryRegistration_h

#include <stdio.h>
#include <stdlib.h>

#include "Factory.h"

// Macro to declare a function as "statically initialized", i.e. to be called at load-time.
// In case this is not supported by a given compiler, we can still revert to manually calling registration functions.
#if defined(_MSC_VER)
    // cf. https://stackoverflow.com/a/2390626/649700
    #pragma section(".CRT$XCU", read)
    #define INIT_MSVC_(f,p) \
            static void f(void); \
            __declspec(allocate(".CRT$XCU")) void (*f##_)(void) = f; \
            __pragma(comment(linker,"/include:" p #f "_")) \
            static void f(void)
    #ifdef _WIN64
        #define STATIC_INIT(f) INIT_MSVC_(f,"")
    #else
        #define STATIC_INIT(f) INIT_MSVC_(f,"_")
    #endif
#else
    // Supported by GCC, Clang, ...
    #define STATIC_INIT(f) \
        static void f(void) __attribute__((constructor)); \
        static void f(void)
#endif

/** To be called in each Module's static constructor at load-time.*/
static inline void RegisterInFactory(const char* registeredType, CreatorFunction createFct)
{
    Factory* factory = factory_getInstance();
    factory->registerModule(factory, registeredType, createFct);
}

#endif /* FactoryRegistration_h */
