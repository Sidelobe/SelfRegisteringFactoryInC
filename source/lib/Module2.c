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

static const char* module2_getType()
{
    return "Module2";
}

typedef struct Module2_Config
{
    int numInChannels;
    int numOutChannels;
    float paramB;
} Module2_Config;

typedef struct Module2_State
{
    Module2_Config m_cfg;
    float m_gainL;
    float m_gainR;
    
} Module2_State;

// Module API functions

static int module_sizeofCfg(void)
{
    return sizeof(Module2_Config);
}

static int module_sizeofState(void)
{
    return sizeof(Module2_State);
}

static bool module_setCfgParam(void* cfgStruct, int cfgParamId, const void* cfgParamValue)
{
    Module2_Config* cfg = (Module2_Config*) cfgStruct;
    
    switch ((Module2_ConfigIDs) cfgParamId)
    {
        case Module2_CfgID_numInChannels_i:
            cfg->numInChannels = *((int*)cfgParamValue);
            break;
        case Module2_CfgID_numOutChannels_i:
            cfg->numOutChannels = *((int*)cfgParamValue);
            break;
        case Module2_CfgID_paramB_f:
            cfg->paramB = *((float*)cfgParamValue);;
            break;
        default:
            return false;
    }
    return true;
}

static bool module_init(void** moduleImpl, void* cfg)
{
    Module2_State* state = (Module2_State*) malloc(sizeof(Module2_State));
    if (state == NULL) { return false; }
    
    *moduleImpl = state;
    memcpy(&state->m_cfg, cfg, sizeof(Module2_Config));
    
    state->m_gainL = 0.33f;
    state->m_gainR = 1.0f - state->m_gainL;
    
    return true;
}

static void module_destroy(void* moduleImpl)
{
    free(moduleImpl);
}

static void module_process(void* moduleImpl, const float* const* audioIn, float** audioOut, const int blockSize)
{
    Module2_State* this_ = (Module2_State*) moduleImpl;
    assert(this_->m_cfg.numInChannels <= this_->m_cfg.numOutChannels);
    
    // do something with object and I/O data
    for (int ch = 0; ch < this_->m_cfg.numOutChannels; ++ch) {
        float gain = (ch%2 == 0) ? this_->m_gainL : this_->m_gainR;
        for (int s = 0; s < blockSize; ++s) {
            audioOut[ch][s] = gain * audioIn[ch % this_->m_cfg.numInChannels][s];
        }
    }
}

static const char* module_getType()
{
    return module2_getType();
}

// Registration in Factory

static Module* module2_creatorFunction()
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

STATIC_INIT(module2_register)
{
    RegisterInFactory(module2_getType(), &module2_creatorFunction);
}
