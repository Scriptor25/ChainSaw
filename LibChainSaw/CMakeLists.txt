find_package(LLVM 17.0.6 REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

message(STATUS "LLVM Available Libs: ${LLVM_AVAILABLE_LIBS}")

set(llvm_libs ${LLVM_AVAILABLE_LIBS})

if(LLVM_ENABLE_FFI)
    find_package(FFI REQUIRED)
    list(APPEND llvm_libs FFI)
endif()
if(LLVM_ENABLE_LIBEDIT)
    find_package(LibEdit REQUIRED)
    list(APPEND llvm_libs LibEdit)
endif()
if(LLVM_ENABLE_TERMINFO)
    find_package(Terminfo REQUIRED)
    list(APPEND llvm_libs Terminfo)
endif()
if(LLVM_ENABLE_ZLIB)
    find_package(ZLIB REQUIRED)
    list(APPEND llvm_libs ZLIB)
endif()
if(LLVM_ENABLE_ZSTD)
    find_package(zstd REQUIRED)
    list(APPEND llvm_libs zstd)
endif()
if(LLVM_ENABLE_LIBXML2)
    find_package(LibXml2 REQUIRED)
    list(APPEND llvm_libs LibXml2)
endif()
if(LLVM_ENABLE_CURL)
    find_package(CURL REQUIRED)
    list(APPEND llvm_libs CURL)
endif()

set(CHAINSAW_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CHAINSAW_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CHAINSAW_VERSION_PATCH ${PROJECT_VERSION_PATCH})

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/config/version.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/config/version.h
)

file(GLOB_RECURSE src src/**.cpp include/**.hpp)
add_library(LibChainSaw STATIC ${src})
target_include_directories(LibChainSaw PUBLIC include ${CMAKE_CURRENT_BINARY_DIR} ${LLVM_INCLUDE_DIRS})
target_compile_definitions(LibChainSaw PUBLIC ${LLVM_DEFINITIONS})
target_link_directories(LibChainSaw PUBLIC ${LLVM_LIBRARY_DIRS})
target_link_libraries(LibChainSaw PUBLIC ${llvm_libs})
