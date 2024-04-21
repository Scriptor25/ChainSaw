file(GLOB_RECURSE ChainSaw_src "src/**.c" "src/**.cpp" "include/**.h" "include/**.hpp")
add_executable(ChainSaw ${ChainSaw_src})
target_include_directories(ChainSaw PUBLIC "include")
