find_package(LLVM 17.0.6 REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

llvm_map_components_to_libnames(llvm_libs core support analysis passes X86 orcjit)

file(GLOB_RECURSE src src/**.cpp include/**.hpp)
add_library(LibChainSaw STATIC ${src})
target_include_directories(LibChainSaw PUBLIC include ${LLVM_INCLUDE_DIRS})
target_compile_definitions(LibChainSaw PUBLIC ${LLVM_DEFINITIONS})
target_link_libraries(LibChainSaw PUBLIC ${llvm_libs})
