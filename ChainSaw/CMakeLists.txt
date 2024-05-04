find_package(LLVM REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

file(GLOB_RECURSE src src/**.cpp include/**.hpp)
add_executable(ChainSaw ${src})
target_include_directories(ChainSaw PUBLIC include)

llvm_map_components_to_libnames(llvm_libs core)
target_link_libraries(ChainSaw PUBLIC ${llvm_libs})

install(TARGETS ChainSaw)
