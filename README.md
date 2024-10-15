```
   ▗▄▄▗▄▄▄▗▖  ▗▄▄▄▖    ▗▄▄▖▗▄▄▄▖▗▄▄▗▄▄▄▖▗▄▄▗▄▄▄▗▄▄▄▗▄▄▖▗▄▄▄▗▖  ▗▖▗▄▄▖                 
  ▐▌  ▐▌  ▐▌  ▐▌       ▐▌ ▐▐▌  ▐▌    █ ▐▌    █ ▐▌  ▐▌ ▐▌ █ ▐▛▚▖▐▐▌                    
   ▝▀▚▐▛▀▀▐▌  ▐▛▀▀▘    ▐▛▀▚▐▛▀▀▐▌▝▜▌ █  ▝▀▚▖ █ ▐▛▀▀▐▛▀▚▖ █ ▐▌ ▝▜▐▌▝▜▌                 
  ▗▄▄▞▐▙▄▄▐▙▄▄▐▌       ▐▌ ▐▐▙▄▄▝▚▄▞▗▄█▄▗▄▄▞▘ █ ▐▙▄▄▐▌ ▐▗▄█▄▐▌  ▐▝▚▄▞▘                 
  ▗▄▄▄▖▗▄▖ ▗▄▄▗▄▄▄▗▄▖▗▄▄▗▖  ▗▖    ▗▄▄▖ ▗▄▗▄▄▄▗▄▄▄▗▄▄▄▗▄▄▖▗▖  ▗▖    ▗▄▄▄▗▖  ▗▖     ▗▄▄▖
  ▐▌  ▐▌ ▐▐▌    █▐▌ ▐▐▌ ▐▝▚▞▘     ▐▌ ▐▐▌ ▐▌█   █ ▐▌  ▐▌ ▐▐▛▚▖▐▌      █ ▐▛▚▖▐▌    ▐▌   
  ▐▛▀▀▐▛▀▜▐▌    █▐▌ ▐▐▛▀▚▖▐▌      ▐▛▀▘▐▛▀▜▌█   █ ▐▛▀▀▐▛▀▚▐▌ ▝▜▌      █ ▐▌ ▝▜▌    ▐▌   
  ▐▌  ▐▌ ▐▝▚▄▄▖ █▝▚▄▞▐▌ ▐▌▐▌      ▐▌  ▐▌ ▐▌█   █ ▐▙▄▄▐▌ ▐▐▌  ▐▌    ▗▄█▄▐▌  ▐▌    ▝▚▄▄▖                                                                                

```

### Example code that implements a self-registering factory pattern in C

![](https://img.shields.io/github/license/Sidelobe/SelfRegisteringFactoryInC)
![](https://img.shields.io/badge/C-99-blue.svg?style=flat&logo=c%2B%2B)
![](https://img.shields.io/badge/dependencies-none-blue)

## Design

The Factory Pattern decouples the callers from the factory products (calling them "modules" in this example) on a code level, but the factory itself still needs to know about (i.e. include) all products.

By making these modules "self-registering", we remove this dependency as well, creating **complete decoupling** on code level. This design brings the following, mayor advantage: **it allows for the modules to be added or removed at compile time without making a single change in the code.**

This example assumes we have a series of "modules" that implement some given audio functionality. Each module has a potentially unique set of configuration parameters needed for its instantiation ("constructor arguments"). Some of these config params are probably common to all modules, e.g. the number of input and output channels.

### Design goals/thoughts

* The modules *need not be included anywhere*. I made this obvious by not even giving them header files
* The design aims to be mostly Object-Oriented
* To build a module, the factory needs to be provided a set of configuration parameters. The `structs` that store these are not known neither to the factory nor the client code.
* The config parameters are set using the pair `{ID, value}`. I've used `enums` and Hungarian notation to embed type information `_f` for floats, `_i` for ints. Of course, generic data structures like XML or JSON could be used instead.
* Since the call order of the functions at load-time is not guaranteed, we rely on the Factory being a global object, initialized & allocated by the first caller, whichever that may be. For this, we use a variant of the *Singleton pattern*).

### Portability

Portability hinges on the ability of the compiler/linker in providing a way of executing code at load-time, i.e. before `main()` is called. In C++, this can be done with the constructor of a static object. In C, there's no 'native' language support for this.

Clang and GCC allow to do this by qualifying functions with `__attribute__((constructor))`. With the Microsoft Visual Studio Compiler (MSVC), which historically has notoriously bad support for C, things are quite a bit more complicated (cf. [FactoryRegistration.h](source/lib/FactoryRegistration.h)) -- but possible. In a nutshell, we need to tell the linker to place these functions in a specific section (e.g. `.init`), so they are called at load-time before `main()`.

I have tested with following embedded compilers:

- TI C6000 Series Compiler: supports `__attribute__((constructor))`


#### Further reading

These resources provide useful information on the topic:

- Matt Godbolt's fantastic talk ["The Bits Between the Bits: How We Get to main()"](https://www.youtube.com/watch?v=dOfucXtyEsU)
- [https://wiki.osdev.org/Calling\_Global\_Constructors](https://wiki.osdev.org/Calling_Global_Constructors)
- [https://embeddedartistry.com/blog/2019/04/08/a-general-overview-of-what-happens-before-main/](https://embeddedartistry.com/blog/2019/04/08/a-general-overview-of-what-happens-before-main/)

### Compiler/Linker Settings - Dealing with Optimization

Under certain circumstances, the compiler will optimize away the registration functions because they are not called anywhere. If the code is in a static library, it is even more likely the factory will suddenly be empty in release builds.

To specifically test this case, I've put the example code in a static library, which is linked against `main.c` (executable).

To avoid the "optimizing away", I've used the following linker options in Cmake on the exectuble ("App") target -- individually for Clang, GCC and MSVC:

```cmake
if (CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_link_options(${PROJECT_NAME}App PUBLIC "LINKER:-force_load,$<TARGET_FILE:${PROJECT_NAME}Lib>")
elseif (MSVC)
    set_target_properties(${PROJECT_NAME}App PROPERTIES LINK_FLAGS "/WHOLEARCHIVE:${PROJECT_NAME}Lib")
else () # GCC and others
    target_link_options(${PROJECT_NAME}App PUBLIC "LINKER:--whole-archive,$<TARGET_FILE:${PROJECT_NAME}Lib>,--no-whole-archive")
endif()
```

### Build Status / Quality Metrics

Tested with (Debug & Release builds):

* macos : clang
* linux : gcc 9 & 12
* windows : msvc

![](https://img.shields.io/badge/branch-main-blue)
[![Build & Test](https://github.com/Sidelobe/SelfRegisteringFactoryInC/actions/workflows/workflow.yml/badge.svg?branch=main)](https://github.com/Sidelobe/SelfRegisteringFactoryInC/actions/workflows/workflow.yml)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
