#include <runtime/Compute.h>
#include <runtime/Operators.h>
#include <lang/Stmt.h>
#include <lang/Expr.h>
#include <lang/Parser.h>

#include <cassert>
#include <filesystem>
#include <fstream>

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::StmtPtr stmt)
{
	if (auto s = std::dynamic_pointer_cast<csaw::lang::FunStmt>(stmt))
		return Evaluate(env, s);
	if (auto s = std::dynamic_pointer_cast<csaw::lang::RetStmt>(stmt))
		return Evaluate(env, s);
	if (auto s = std::dynamic_pointer_cast<csaw::lang::EnclosedStmt>(stmt))
		return Evaluate(env, s);
	if (auto s = std::dynamic_pointer_cast<csaw::lang::ForStmt>(stmt))
		return Evaluate(env, s);
	if (auto s = std::dynamic_pointer_cast<csaw::lang::VarStmt>(stmt))
		return Evaluate(env, s);
	if (auto s = std::dynamic_pointer_cast<csaw::lang::IncStmt>(stmt))
		return Evaluate(env, s);
	if (auto s = std::dynamic_pointer_cast<csaw::lang::WhileStmt>(stmt))
		return Evaluate(env, s);
	if (auto s = std::dynamic_pointer_cast<csaw::lang::IfStmt>(stmt))
		return Evaluate(env, s);
	if (auto s = std::dynamic_pointer_cast<csaw::lang::ThingStmt>(stmt))
		return Evaluate(env, s);
	if (auto s = std::dynamic_pointer_cast<csaw::lang::AliasStmt>(stmt))
		return Evaluate(env, s);

	if (auto expr = std::dynamic_pointer_cast<csaw::lang::Expr>(stmt))
		return Evaluate(env, expr);

	throw;
}

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::FunStmtPtr stmt)
{
	std::vector<std::string> params;
	for (auto& param : stmt->Params)
		params.push_back(param.second);

	FunDefinition definition =
		[constructor = stmt->Constructor, result = stmt->Result, params = stmt->Params, body = stmt->Body]
		(EnvironmentPtr env, ValuePtr callee, const std::vector<ValuePtr>& args) -> ValuePtr
		{
			auto e = Environment::Ptr(env);
			if (callee)
				e->CreateVariable("my", callee->GetType(), callee);
			if (constructor)
				e->CreateVariable("my", result, ValuePtr());

			size_t i = 0;
			for (; i < params.size(); i++)
				e->CreateVariable(params[i].first, params[i].second, args[i]);
			for (; i < args.size(); i++)
				e->PushVarArg(args[i]);

			auto value = Evaluate(e, body);

			if (result.empty())
				assert(!value || !value->IsReturned());
			else
			{
				if (constructor)
					assert(!value);
				else
				{
					assert(value && value->IsReturned());
					value->MarkReturned(false);
				}
			}

			if (constructor)
				return e->GetVariable("my");

			return value;
		};

	env->CreateFunction(stmt->Constructor, stmt->Name, stmt->Result, params, stmt->VarArg, stmt->Callee, definition);
	return ValuePtr();
}

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::RetStmtPtr stmt)
{
	auto value = Evaluate(env, stmt->Value);
	value->MarkReturned(true);
	return value;
}

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::EnclosedStmtPtr stmt)
{
	auto contentenv = Environment::Ptr(env);
	for (auto& s : stmt->Content)
	{
		auto value = Evaluate(contentenv, s);
		if (value && value->IsReturned())
			return value;
	}
	return ValuePtr();
}

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::ForStmtPtr stmt)
{
	auto headerenv = Environment::Ptr(env);
	for (Evaluate(headerenv, stmt->Pre); EvalOpBool(headerenv, Evaluate(headerenv, stmt->Condition)); Evaluate(headerenv, stmt->Loop))
	{
		auto value = Evaluate(headerenv, stmt->Body);
		if (value && value->IsReturned())
			return value;
	}
	return ValuePtr();
}

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::VarStmtPtr stmt)
{
	return env->CreateVariable(stmt->Name, stmt->Type, stmt->Value ? Evaluate(env, stmt->Value) : ValuePtr());
}

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::IncStmtPtr stmt)
{
	auto prev = env->GetFilepath();
	auto path = std::filesystem::path(prev).parent_path();
	path = path / stmt->Filename;

	std::ifstream stream(path);
	assert(stream.is_open());

	env->SetFilepath(path.string());
	csaw::lang::Parser::Parse(stream, env);
	env->SetFilepath(prev);

	return ValuePtr();
}

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::WhileStmtPtr stmt)
{
	while (EvalOpBool(env, Evaluate(env, stmt->Condition)))
	{
		auto value = Evaluate(env, stmt->Body);
		if (value && value->IsReturned())
			return value;
	}
	return ValuePtr();
}

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::IfStmtPtr stmt)
{
	if (EvalOpBool(env, Evaluate(env, stmt->Condition)))
		return Evaluate(env, stmt->True);

	if (stmt->False)
		return Evaluate(env, stmt->False);

	return ValuePtr();
}

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::ThingStmtPtr stmt)
{
	Environment::CreateThing(stmt->Name, stmt->Group, stmt->Elements);
	return ValuePtr();
}

csaw::runtime::ValuePtr csaw::runtime::Evaluate(EnvironmentPtr env, const csaw::lang::AliasStmtPtr stmt)
{
	Environment::CreateAlias(stmt->Name, stmt->Origin);
	return ValuePtr();
}
