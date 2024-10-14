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

#include <assert.h>
#include <string.h> // from memcpy

#include "Module.h"
#include "ModuleConfigParamIDs.h"
#include "FactoryRegistration.h"

static const char* module1_getType()
{
    return "Module1";
}

typedef struct Module1_Config
{
    int numInChannels;
    int numOutChannels;
    float paramA;
} Module1_Config;

typedef struct Module1_State
{
    Module1_Config m_cfg;
    float m_gain;
    
} Module1_State;

// Module API functions

static int module_sizeofCfg(void)
{
    return sizeof(Module1_Config);
}

static int module_sizeofState(void)
{
    return sizeof(Module1_State);
}

static bool module_setCfgParam(void* cfgStruct, int cfgParamId, const void* cfgParamValue)
{
    Module1_Config* cfg = (Module1_Config*) cfgStruct;
    
    switch ((Module1_ConfigIDs) cfgParamId)
    {
        case Module1_CfgID_numInChannels_i:
            cfg->numInChannels = *((int*)cfgParamValue);
            break;
        case Module1_CfgID_numOutChannels_i:
            cfg->numOutChannels = *((int*)cfgParamValue);
            break;
        case Module1_CfgID_paramA_f:
            cfg->paramA = *((float*)cfgParamValue);;
            break;
        default:
            return false;
    }
    return true;
}

static bool module_init(void** moduleImpl, void* cfg)
{
    Module1_State* state = (Module1_State*) malloc(sizeof(Module1_State));
    if (state == NULL) { return false; }
    
    *moduleImpl = state;
    memcpy(&state->m_cfg, cfg, sizeof(Module1_Config));
   
    state->m_gain = 0.707f;
    
    return true;
}

static void module_destroy(void* moduleImpl)
{
    free(moduleImpl);
}

static void module_process(void* moduleImpl, const float* const* audioIn, float** audioOut, const int blockSize)
{
    Module1_State* this_ = (Module1_State*) moduleImpl;
    assert(this_->m_cfg.numInChannels <= this_->m_cfg.numOutChannels);
    
    // do something with object and I/O data
    for (int ch = 0; ch < this_->m_cfg.numOutChannels; ++ch) {
        for (int s = 0; s < blockSize; ++s) {
            audioOut[ch][s] = this_->m_gain * audioIn[ch % this_->m_cfg.numInChannels][s];
        }
    }
}

static const char* module_getType()
{
    return module1_getType();
}

// Registration in Factory

static Module* module1_creatorFunction()
{
    Module* module = (Module*) malloc(sizeof(Module));
    
    module->module_sizeofCfg = &module_sizeofCfg;
    module->module_setCfgParam = &module_setCfgParam;
    module->module_init = &module_init;
    module->module_destroy = &module_destroy;
    module->module_process = &module_process;
    module->module_getType = &module_getType;
    
    return module;
}

STATIC_INIT(module1_register)
{
    RegisterInFactory(module1_getType(), &module1_creatorFunction);
}
