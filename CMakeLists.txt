cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(ChainSaw VERSION 1.0.0)
add_subdirectory(PCG)
add_subdirectory(LibChainSaw)
add_subdirectory(ChainSawCLI)

install(DIRECTORY csaw/std/include DESTINATION ${CMAKE_INSTALL_PREFIX})
