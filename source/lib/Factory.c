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

#include "Factory.h"

#include <stdbool.h>
#include <stdio.h> // strcmp
#include <string.h> // memset

/** Initializes the Factory object */
static void factory_create(struct Factory* factory);

Factory* factory_getInstance()
{
    static Factory* factory = NULL;
    if (factory == NULL) {
        factory = (Factory*) malloc(sizeof(Factory)); // allocate
        factory_create(factory);                      // initialize
    }
    return factory;
}

void registerModule(struct Factory* factory, const char* registeredType, CreatorFunction createFct)
{
    if (factory->numRegisteredModules >= FACTORY_MAX_REGISTERED_MODULES) {
        printf("ERROR: Factory is overloaded: registry size cannot accomodate all modules!\n");
        return;
    }
    
    bool isDuplicate = false;
    for (int i=0; i < factory->numRegisteredModules; ++i) {
        if (strcmp(factory->registry[i].registeredType, registeredType) == 0) {
            isDuplicate = true;
            break;
        }
    }
    if (isDuplicate) {
        printf("ERROR: Cannot register same type twice!\n", registeredType);
        return;
    }
    
    // Append tos end of registry
    printf("Registering Module type: %s\n", registeredType);
    factory->registry[factory->numRegisteredModules].creatorFct = createFct;
    factory->registry[factory->numRegisteredModules].registeredType = registeredType;
    factory->numRegisteredModules++;
}

struct Module* buildModule(struct Factory* factory, const char* moduleType)
{
    for (int i=0; i < factory->numRegisteredModules; ++i) {
        if (strcmp(factory->registry[i].registeredType, moduleType) == 0) {
            return factory->registry[i].creatorFct();
        }
    }
    printf("Error: Factory could not build module of type %s", moduleType);
    return NULL;
}

void printInventory(struct Factory* factory)
{
    printf("Factory can build: \n");
    for (int i=0; i < factory->numRegisteredModules; ++i) {
        printf("\t- %s\n", factory->registry[i].registeredType);
    }
}

void factory_create(struct Factory* factory)
{
    factory->registerModule = &registerModule;
    factory->buildModule = &buildModule;
    factory->printInventory = &printInventory;
    
    memset(factory->registry, 0, FACTORY_MAX_REGISTERED_MODULES * sizeof(FactoryRegistrationEntry));
    factory->numRegisteredModules = 0;
}
