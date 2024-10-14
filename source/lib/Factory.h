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

#ifndef Factory_h
#define Factory_h

#include <stdio.h>
#include <stdlib.h>

// The maximum number of modules that can register in the factory
#define FACTORY_MAX_REGISTERED_MODULES 50

struct Module; // forward
typedef struct Module* (*CreatorFunction)(void);

typedef struct FactoryRegistrationEntry
{
    const char* registeredType;
    CreatorFunction creatorFct;
} FactoryRegistrationEntry;

/** 
 * Public object for Factory, which is able to construct modules without concrete knowledge of them.
 * @note Could use PIMPL idiom to make this opaque for better encapsulation.
 */
typedef struct Factory
{
    void (*registerModule)(struct Factory* factory, const char* registeredType, CreatorFunction createFct);
    struct Module* (*buildModule)(struct Factory* factory, const char* moduleType);
    void (*printInventory)(struct Factory* factory);

    FactoryRegistrationEntry registry[FACTORY_MAX_REGISTERED_MODULES];
    int numRegisteredModules;
    
} Factory;

/** Singleton Pattern - the first caller allocates the Factory. This can/will happen in one of the static initializers. */
Factory* factory_getInstance();


#endif /* Factory_h */
