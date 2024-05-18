cmake_minimum_required(VERSION 3.22)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ChainSaw VERSION 1.0.0)
add_subdirectory(LibChainSaw)
add_subdirectory(ChainSawCLI)
