file(GLOB_RECURSE src "src/**.c" "src/**.cpp" "include/**.h" "include/**.hpp")
add_executable(ChainSaw ${src})
target_include_directories(ChainSaw PUBLIC "include")

install(TARGETS ChainSaw)
