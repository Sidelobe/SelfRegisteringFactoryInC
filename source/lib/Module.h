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

#ifndef Module_h
#define Module_h

#include <stdio.h>
#include <stdbool.h>

typedef struct Module
{
    // MARK: - public functions called *before* the object is created
    /** @return the size of the config struct, in bytes */
    int (*module_sizeofCfg)(void);
    
    /**
     * Generic function to set parameters in a Module's *opaque* config parameter struct
     * @note Consider using a union for the value
     * @return true upon success, false if any error occurs
     */
    bool (*module_setCfgParam)(void* cfgStruct, int cfgParamId, const void* cfgParamValue);
    
    // MARK: - public functions called on the module object
    /**
     * Initialize the module based on the parameters in the supplied config struct
     *
     * @param moduleImpl Pointer-to-Pointer to the allocated, but not yet initialized module
     * @param cfg Pointer to the config structure, which is was allocated with module_sizeofCfg() and 
     *            initialized with module_setCfgParam() calls. Its contents will be copied.
     *
     * @return true upon success, false if any error occurs
     */
    bool (*module_init)(void** moduleImpl, void* cfg);
    
    /** destroy the module, freeing up all its owned resources */
    void (*module_destroy)(void* moduleImpl);
    
    /** process one block of audio samples. Number of channels should be configured upon initialization */
    void (*module_process)(void* moduleImpl, const float* const* audioIn, float** audioOut, const int blockSize);
    
    /** @return the type name of the module, which is the name used for registration in the Factory */
    const char* (*module_getType)(void);
    
    // MARK: - public 'member variables'
    /** Pointer to implementation (opaque, PIMPL idiom) - This will point to the state struct after initialization */
    void* moduleImpl;
    
} Module;


#endif /* Module_h */
