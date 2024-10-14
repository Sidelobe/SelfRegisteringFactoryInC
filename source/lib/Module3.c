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

static const char* module3_getType()
{
    return "Module3";
}

typedef struct Module3_Config
{
    int numInChannels;
    int numOutChannels;
    float paramC;
    float paramD;
} Module3_Config;

typedef struct Module3_State
{
    Module3_Config m_cfg;
    float m_gain;
    
} Module3_State;

// Module API functions

static int module_sizeofCfg(void)
{
    return sizeof(Module3_Config);
}

static int module_sizeofState(void)
{
    return sizeof(Module3_State);
}

static bool module_setCfgParam(void* cfgStruct, int cfgParamId, const void* cfgParamValue)
{
    Module3_Config* cfg = (Module3_Config*) cfgStruct;
    
    switch ((Module3_ConfigIDs) cfgParamId)
    {
        case Module3_CfgID_numInChannels_i:
            cfg->numInChannels = *((int*)cfgParamValue);
            break;
        case Module3_CfgID_numOutChannels_i:
            cfg->numOutChannels = *((int*)cfgParamValue);
            break;
        case Module3_CfgID_paramC_f:
            cfg->paramC = *((float*)cfgParamValue);;
            break;
        case Module3_CfgID_paramD_f:
            cfg->paramD = *((float*)cfgParamValue);;
            break;
        default:
            return false;
    }
    return true;
}

static bool module_init(void** moduleImpl, void* cfg)
{
    Module3_State* state = (Module3_State*) malloc(sizeof(Module3_State));
    if (state == NULL) { return false; }
    
    *moduleImpl = state;
    memcpy(&state->m_cfg, cfg, sizeof(Module3_Config));
    
    state->m_gain = 0.707f;
    
    return true;
}

static void module_destroy(void* moduleImpl)
{
    free(moduleImpl);
}

static void module_process(void* moduleImpl, const float* const* audioIn, float** audioOut, const int blockSize)
{
    Module3_State* this_ = (Module3_State*) moduleImpl;
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
    return module3_getType();
}

// Registration in Factory

static Module* module3_creatorFunction()
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

STATIC_INIT(module3_register)
{
    RegisterInFactory(module3_getType(), &module3_creatorFunction);
}
