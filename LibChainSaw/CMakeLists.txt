find_package(LLVM 17.0.6 REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(llvm_libs)
list(APPEND llvm_libs
        LLVMDemangle
        LLVMSupport
        LLVMTableGen
        LLVMTableGenGlobalISel
        LLVMTableGenCommon
        LLVMCore
        LLVMFuzzerCLI
        LLVMFuzzMutate
        LLVMFileCheck
        LLVMInterfaceStub
        LLVMIRPrinter
        LLVMIRReader
        LLVMIRReader
        LLVMCodeGenTypes
        LLVMCodeGen
        LLVMSelectionDAG
        LLVMAsmPrinter
        LLVMMIRParser
        LLVMGlobalISel
        LLVMBinaryFormat
        LLVMBitReader
        LLVMBitWriter
        LLVMBitstreamReader
        LLVMDWARFLinker
        LLVMDWARFLinkerParallel
        LLVMExtensions
        LLVMFrontendHLSL
        LLVMFrontendOpenACC
        LLVMFrontendOpenMP
        LLVMTransformUtils
        LLVMInstrumentation
        LLVMAggressiveInstCombine
        LLVMInstCombine
        LLVMScalarOpts
        LLVMipo
        LLVMVectorize
        LLVMObjCARCOpts
        LLVMCoroutines
        LLVMCFGuard
        LLVMLinker
        LLVMAnalysis
        LLVMLTO
        LLVMMC
        LLVMMCParser
        LLVMMCDisassembler
        LLVMMCA
        LLVMObjCopy
        LLVMObject
        LLVMObjectYAML
        LLVMOption
        LLVMRemarks
        LLVMDebuginfod
        LLVMDebugInfoDWARF
        LLVMDebugInfoGSYM
        LLVMDebugInfoLogicalView
        LLVMDebugInfoMSF
        LLVMDebugInfoCodeView
        LLVMDebugInfoPDB
        LLVMSymbolize
        LLVMDebugInfoBTF
        LLVMDWP
        LLVMExecutionEngine
        LLVMInterpreter
        LLVMJITLink
        LLVMMCJIT
        LLVMOrcJIT
        LLVMOrcShared
        LLVMOrcTargetProcess
        LLVMRuntimeDyld
        LLVMPerfJITEvents
        LLVMTarget
        LLVMAsmParser
        LLVMLineEditor
        LLVMProfileData
        LLVMCoverage
        LLVMPasses
        LLVMTargetParser
        LLVMTextAPI
        LLVMDlltoolDriver
        LLVMLibDriver
        LLVMXRay
        LLVMWindowsDriver
        LLVMWindowsManifest
        LTO
        LLVMBOLTCore
        LLVMBOLTPasses
        LLVMBOLTProfile
        LLVMBOLTRewrite
        LLVMBOLTRuntimeLibs
        LLVMBOLTUtils
        LLVMCFIVerify
        LLVMDiff
        LLVMExegesis
        Remarks
        Polly
)

foreach (target ${LLVM_TARGETS_TO_BUILD})
    list(APPEND llvm_libs
            LLVM${target}CodeGen
            LLVM${target}AsmParser
            LLVM${target}Disassembler
            LLVM${target}TargetMCA
            LLVM${target}Desc
            LLVM${target}Info
            LLVM${target}Utils
            LLVMExegesis${target}
            LLVMBOLTTarget${target}
    )
endforeach ()

# check if library exists
foreach (lib ${llvm_libs})
    if (NOT ${lib} IN_LIST LLVM_AVAILABLE_LIBS)
        message(WARNING "Removing unknown lib '${lib}' from LLVM libs")
        list(REMOVE_ITEM llvm_libs ${lib})
    endif ()
endforeach ()

message(STATUS "Using LLVM Components: ${llvm_libs}")

if (LLVM_ENABLE_FFI)
    find_package(FFI REQUIRED)
    list(APPEND llvm_libs ${FFI_LIBRARIES})
endif ()
if (LLVM_ENABLE_LIBEDIT)
    find_package(LibEdit REQUIRED)
    list(APPEND llvm_libs ${LibEdit_LIBRARIES})
endif ()
if (LLVM_ENABLE_TERMINFO)
    find_package(Terminfo REQUIRED)
    list(APPEND llvm_libs ${Terminfo_LIBRARIES})
endif ()
if (LLVM_ENABLE_ZLIB)
    find_package(ZLIB REQUIRED)
    list(APPEND llvm_libs ${ZLIB_LIBRARIES})
endif ()
if (LLVM_ENABLE_ZSTD)
    find_package(zstd REQUIRED)
    list(APPEND llvm_libs ${zstd_LIBRARIES})
endif ()
if (LLVM_ENABLE_LIBXML2)
    find_package(LibXml2 REQUIRED)
    list(APPEND llvm_libs ${LibXml2_LIBRARIES})
endif ()
if (LLVM_ENABLE_CURL)
    find_package(CURL REQUIRED)
    list(APPEND llvm_libs ${CURL_LIBRARIES})
endif ()

separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})

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
target_compile_definitions(LibChainSaw PUBLIC ${LLVM_DEFINITIONS_LIST})
target_link_directories(LibChainSaw PUBLIC ${LLVM_LIBRARY_DIRS})
target_link_libraries(LibChainSaw PUBLIC PCG ${llvm_libs})
