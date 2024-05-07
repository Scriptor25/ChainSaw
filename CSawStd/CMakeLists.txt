file(GLOB_RECURSE src src/**.cpp include/**.hpp)
add_library(CSawStd STATIC ${src})
target_include_directories(CSawStd PUBLIC include)
