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

// for MSVC, do before including math.h
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "Factory.h"
#include "Module.h"
#include "ModuleConfigParamIDs.h"

#define BLOCKSIZE 64

// NOTE how none of the concrete modules is included here

// only their types need to be known
const char* moduleTypes[3] = { "Module1", "Module2", "Module3" };

int main()
{
    Factory* factory = factory_getInstance();
    factory->printInventory(factory);
    assert(factory->numRegisteredModules == 3);
    
    const int numInCh = 2;
    const int numOutCh = 4;
    const float A = (float)M_PI_4;
    const float B = (float)-M_PI_4;
    const float C = (float)M_SQRT2;
    const float D = (float)M_SQRT1_2;
    
    Module* modules[3];
    
    // Sub-scope to show required lifetime of data (and test it)
    {
        void* moduleConfigs[3];
        
        for (int i=0; i < 3; ++i) {
            modules[i] = factory->buildModule(factory, moduleTypes[i]);
            assert(modules[i] != NULL);
            moduleConfigs[i] = malloc(modules[i]->module_sizeofCfg());
        }
        
        // generic way of passing common and individual config parameters
        for (int i=0; i < 3; ++i) {
            modules[i]->module_setCfgParam(moduleConfigs[i], 0, &numInCh);
            modules[i]->module_setCfgParam(moduleConfigs[i], 1, &numOutCh);
        }
        modules[0]->module_setCfgParam(moduleConfigs[0], 2, &A);
        modules[1]->module_setCfgParam(moduleConfigs[1], 2, &B);
        modules[2]->module_setCfgParam(moduleConfigs[2], 2, &C);
        modules[2]->module_setCfgParam(moduleConfigs[2], 3, &D);
        
        bool success = true;
        for (int i=0; i < 3; ++i) {
            success = success && modules[i]->module_init((void**)&modules[i]->moduleImpl, moduleConfigs[i]);
        }
        assert(success);
        
        // Config structs are no longer needed, as modules will copy their content.
        for (int i=0; i < 3; ++i) {
            free(moduleConfigs[i]);
        }
    }
    
    {
        float buffer1[BLOCKSIZE];
        float buffer2[BLOCKSIZE];
        float buffer3[BLOCKSIZE];
        float buffer4[BLOCKSIZE];
        float* buffer[4] = { buffer1, buffer2, buffer3, buffer4 };
        
        modules[0]->module_process(modules[0]->moduleImpl, (const float**)buffer, buffer, BLOCKSIZE);
        modules[1]->module_process(modules[1]->moduleImpl, (const float**)buffer, buffer, BLOCKSIZE);
        modules[2]->module_process(modules[2]->moduleImpl, (const float**)buffer, buffer, BLOCKSIZE);
        
    }
    
    for (int i=0; i < 3; ++i) {
        modules[i]->module_destroy(modules[i]->moduleImpl);
    }

    return 0;
}
