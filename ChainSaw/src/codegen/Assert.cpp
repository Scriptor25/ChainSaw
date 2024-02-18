#include <codegen/Assert.h>

#include <lang/Stmt.h>

void csaw::codegen::WAssert(const char* expr, ContextPtr context, csaw::lang::StmtPtr ptr)
{
	std::cerr << "Assertion failed: " << expr << std::endl;

	std::cerr << "Line " << ptr->Line << ':' << std::endl;
	std::cerr << ptr << std::endl;

	auto& filepaths = context->Filepaths();
	for (size_t i = 0; i < filepaths.size(); i++)
		std::cerr << "\tin " << filepaths[filepaths.size() - 1 - i] << std::endl;

#ifndef NDEBUG
	throw;
#endif
}
