find_package(LLVM 17.0.6 REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(llvm_libs)
list(APPEND llvm_libs
        LLVMCore
        LLVMSupport
        LLVMIRReader
        LLVMOrcJIT
        LLVMPasses
        LLVMInstrumentation
)
foreach (target ${LLVM_TARGETS_TO_BUILD})
    list(APPEND llvm_libs LLVM${target}CodeGen)
    list(APPEND llvm_libs LLVM${target}AsmParser)
    list(APPEND llvm_libs LLVM${target}Disassembler)
    list(APPEND llvm_libs LLVM${target}TargetMCA)
    list(APPEND llvm_libs LLVM${target}Desc)
    list(APPEND llvm_libs LLVM${target}Info)
    list(APPEND llvm_libs LLVM${target}Utils)
    list(APPEND llvm_libs LLVMExegesis${target})
endforeach ()

# check if library exists
foreach (lib ${llvm_libs})
    if (NOT ${lib} IN_LIST LLVM_AVAILABLE_LIBS)
        message(WARNING "Libs do not contains '${lib}'")
        list(REMOVE_ITEM llvm_libs ${lib})
    endif ()
endforeach ()

message(STATUS "Using LLVM Components: ${llvm_libs}")

#set(llvm_libs ${LLVM_AVAILABLE_LIBS})
#list(REMOVE_ITEM llvm_libs "LLVM")
#
#if(LLVM_ENABLE_FFI)
#    find_package(FFI REQUIRED)
#    list(APPEND llvm_libs ${FFI_LIBRARIES})
#endif()
#if(LLVM_ENABLE_LIBEDIT)
#    find_package(LibEdit REQUIRED)
#    list(APPEND llvm_libs ${LibEdit_LIBRARIES})
#endif()
#if(LLVM_ENABLE_TERMINFO)
#    find_package(Terminfo REQUIRED)
#    list(APPEND llvm_libs ${Terminfo_LIBRARIES})
#endif()
#if(LLVM_ENABLE_ZLIB)
#    find_package(ZLIB REQUIRED)
#    list(APPEND llvm_libs ${ZLIB_LIBRARIES})
#endif()
#if(LLVM_ENABLE_ZSTD)
#    find_package(zstd REQUIRED)
#    list(APPEND llvm_libs ${zstd_LIBRARIES})
#endif()
#if(LLVM_ENABLE_LIBXML2)
#    find_package(LibXml2 REQUIRED)
#    list(APPEND llvm_libs ${LibXml2_LIBRARIES})
#endif()
#if(LLVM_ENABLE_CURL)
#    find_package(CURL REQUIRED)
#    list(APPEND llvm_libs ${CURL_LIBRARIES})
#endif()

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
