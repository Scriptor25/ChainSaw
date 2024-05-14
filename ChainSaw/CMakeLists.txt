file(GLOB_RECURSE src src/**.cpp include/**.hpp)
add_executable(ChainSaw ${src})
target_include_directories(ChainSaw PUBLIC include)
target_link_libraries(ChainSaw PUBLIC LibChainSaw)

install(TARGETS ChainSaw)
