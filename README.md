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

### Design goals/thoughts:

* The modules *need not be included anywhere*. I made this obvious by not even giving them header files
* The design aims to be mostly Object-Oriented
* To build a module, the factory needs to be provided a set of configuration parameters. The `structs` that store these are not known neither to the factory nor the client code.
* The config parameters are set using the pair `{ID, value}`. I've used `enums` and Hungarian notation to embed type information `_f` for floats, `_i` for ints. Of course, generic data structures like XML or JSON could be used instead.

### Build Status / Quality Metrics

![](https://img.shields.io/badge/branch-main-blue)
[![Build & Test](https://github.com/Sidelobe/SelfRegisteringFactoryInC/actions/workflows/workflow.yml/badge.svg?branch=main)](https://github.com/Sidelobe/SelfRegisteringFactoryInC/actions/workflows/workflow.yml)
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
