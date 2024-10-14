cmake_minimum_required(VERSION 3.18)

#   ▗▄▄▗▄▄▄▗▖  ▗▄▄▄▖    ▗▄▄▖▗▄▄▄▖▗▄▄▗▄▄▄▖▗▄▄▗▄▄▄▗▄▄▄▗▄▄▖▗▄▄▄▗▖  ▗▖▗▄▄▖                 
#  ▐▌  ▐▌  ▐▌  ▐▌       ▐▌ ▐▐▌  ▐▌    █ ▐▌    █ ▐▌  ▐▌ ▐▌ █ ▐▛▚▖▐▐▌                    
#   ▝▀▚▐▛▀▀▐▌  ▐▛▀▀▘    ▐▛▀▚▐▛▀▀▐▌▝▜▌ █  ▝▀▚▖ █ ▐▛▀▀▐▛▀▚▖ █ ▐▌ ▝▜▐▌▝▜▌                 
#  ▗▄▄▞▐▙▄▄▐▙▄▄▐▌       ▐▌ ▐▐▙▄▄▝▚▄▞▗▄█▄▗▄▄▞▘ █ ▐▙▄▄▐▌ ▐▗▄█▄▐▌  ▐▝▚▄▞▘                 
#  ▗▄▄▄▖▗▄▖ ▗▄▄▗▄▄▄▗▄▖▗▄▄▗▖  ▗▖    ▗▄▄▖ ▗▄▗▄▄▄▗▄▄▄▗▄▄▄▗▄▄▖▗▖  ▗▖    ▗▄▄▄▗▖  ▗▖     ▗▄▄▖
#  ▐▌  ▐▌ ▐▐▌    █▐▌ ▐▐▌ ▐▝▚▞▘     ▐▌ ▐▐▌ ▐▌█   █ ▐▌  ▐▌ ▐▐▛▚▖▐▌      █ ▐▛▚▖▐▌    ▐▌   
#  ▐▛▀▀▐▛▀▜▐▌    █▐▌ ▐▐▛▀▚▖▐▌      ▐▛▀▘▐▛▀▜▌█   █ ▐▛▀▀▐▛▀▚▐▌ ▝▜▌      █ ▐▌ ▝▜▌    ▐▌   
#  ▐▌  ▐▌ ▐▝▚▄▄▖ █▝▚▄▞▐▌ ▐▌▐▌      ▐▌  ▐▌ ▐▌█   █ ▐▙▄▄▐▌ ▐▐▌  ▐▌    ▗▄█▄▐▌  ▐▌    ▝▚▄▄▖
#                                                                                      

##############################
## PROJECT

set(CMAKE_C_COMPILER "cc")

project(SelfRegFactoryC
        VERSION 1.0.0
        DESCRIPTION "A proof of concept: A Self-Registering Factory pattern in C"
        HOMEPAGE_URL "https://github.com/Sidelobe/SelfRegisteringFactoryInC")


set(CMAKE_C_STANDARD 99)

set(CODE_COVERAGE OFF CACHE BOOL "Build with instrumentation and code coverage")
set(ASAN OFF CACHE BOOL "Build with address sanitizer enabled")


##############################
## LIBRARY
file(GLOB_RECURSE sources "source/lib/*.[h,c]")
add_library(${PROJECT_NAME}Lib STATIC ${sources})
target_include_directories(${PROJECT_NAME}Lib PUBLIC "source/lib")

# Create XCode / VS groups
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sources})

##############################
#### Executable (For testing)
add_executable(${PROJECT_NAME}Test "source/main.c")
target_link_libraries(${PROJECT_NAME}Test PUBLIC ${PROJECT_NAME}Lib)

# This is required for the Factory to be populated
if (CMAKE_C_COMPILER_ID MATCHES "Clang")
    target_link_options(${PROJECT_NAME}Test PUBLIC "LINKER:-force_load,$<TARGET_FILE:${PROJECT_NAME}Lib>")
elseif (MSVC)
    set_target_properties(${PROJECT_NAME}Test PROPERTIES LINK_FLAGS "/WHOLEARCHIVE:${PROJECT_NAME}Lib")
else () # GCC and others
    target_link_options(${PROJECT_NAME}Test PUBLIC "LINKER:--whole-archive,$<TARGET_FILE:${PROJECT_NAME}Lib>,--no-whole-archive")
endif()

## Add exectubale as CTest test
enable_testing()
add_test(NAME ${PROJECT_NAME}Test COMMAND $<TARGET_FILE:${PROJECT_NAME}Test>)
