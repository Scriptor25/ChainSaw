file(GLOB_RECURSE src src/**.cpp include/**.hpp)

add_executable(csaw ${src})
target_include_directories(csaw PUBLIC include)
target_link_libraries(csaw PUBLIC LibChainSaw)

install(TARGETS csaw)
