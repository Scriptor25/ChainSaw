# CMakeList.txt: CMake-Projekt für "ChainSaw". Schließen Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#

# Fügen Sie der ausführbaren Datei dieses Projekts eine Quelle hinzu.
add_executable (ChainSaw "src/ChainSaw.cpp"  "src/lang/Parser.cpp" "src/lang/Token.cpp" "src/lang/Next.cpp" "src/lang/Stmt.cpp" "src/lang/Expr.cpp" "src/lang/ASTDef.cpp" "src/runtime/Runtime.cpp" "src/runtime/Stmt.cpp" "src/runtime/Expr.cpp" "src/runtime/Const.cpp" "src/runtime/Operators.cpp" "src/runtime/Assign.cpp" "src/stb_image_write.cpp" "include/csawstd/Img.h" "src/csawstd/Img.cpp" "include/csawstd/Random.h" "src/csawstd/Random.cpp" "include/csawstd/IO.h" "src/csawstd/IO.cpp" "include/csawstd/Math.h" "src/csawstd/Math.cpp" "include/csawstd/Types.h" "src/csawstd/Types.cpp" "include/csawstd/CSawStd.h" "src/csawstd/CSawStd.cpp")

include_directories ("include")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ChainSaw PROPERTY CXX_STANDARD 20)
endif()

# TODO: Fügen Sie bei Bedarf Tests hinzu, und installieren Sie Ziele.
