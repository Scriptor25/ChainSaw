add_executable (ChainSaw
	"src/ChainSaw.cpp"

	"include/codegen/Context.h"
	"include/codegen/Def.h"
	"include/codegen/CodeGen.h"
	"include/codegen/Instruction.h"
	"include/codegen/Type.h"
	"include/codegen/Value.h"
	"src/codegen/Assert.cpp"
	"src/codegen/Branching.cpp"
	"src/codegen/Builder.cpp"
	"src/codegen/Const.cpp"
	"src/codegen/Context.cpp"
	"src/codegen/CreateAdd.cpp"
	"src/codegen/CreateOp.cpp"
	"src/codegen/CreateSub.cpp"
	"src/codegen/Function.cpp"
	"src/codegen/GenExpr.cpp"
	"src/codegen/GenOp.cpp"
	"src/codegen/GenStmt.cpp"
	"src/codegen/Instruction.cpp"
	"src/codegen/Type.cpp"
	"src/codegen/Value.cpp"

	"include/lang/Def.h"
	"include/lang/Expr.h"
	"include/lang/Parser.h"
	"include/lang/Stmt.h"
	"src/lang/Def.cpp"
	"src/lang/Expr.cpp"
	"src/lang/Next.cpp"
	"src/lang/Parser.cpp"
	"src/lang/Stmt.cpp"
	"src/lang/Token.cpp"

	"include/misc/stb_image_write.h"
	"src/misc/stb_image_write.cpp"

	"include/runtime/Runtime.h"
	"src/runtime/Runtime.cpp"
)

include_directories ("include")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ChainSaw PROPERTY CXX_STANDARD 20)
endif()
