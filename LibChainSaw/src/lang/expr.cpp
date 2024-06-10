#include <string>
#include <csaw/Expr.hpp>
#include <csaw/Parser.hpp>
#include <csaw/Type.hpp>

csaw::Expression::Expression(const SourceLoc& loc)
    : Statement(loc)
{
}

csaw::BinaryExpression::BinaryExpression(const SourceLoc& loc, const std::string& op, const ExpressionPtr& left, const ExpressionPtr& right)
    : Expression(loc), Operator(op), Left(left), Right(right)
{
}

csaw::TypePtr csaw::BinaryExpression::GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args)
{
    auto l = Left->GetType(symbols, false, {}, {});
    auto r = Right->GetType(symbols, false, {}, {});

    if (!l || !r) return nullptr;

    if (const auto type = IdentifierExpression(Loc, Operator).GetType(symbols, true, l, {r}))
    {
        if (!type->IsFunctionPointer()) return nullptr;
        return type->AsPointer()->Base->AsFunction()->Result;
    }
    if (const auto type = IdentifierExpression(Loc, Operator).GetType(symbols, true, {}, {l, r}))
    {
        if (!type->IsFunctionPointer()) return nullptr;
        return type->AsPointer()->Base->AsFunction()->Result;
    }

    if (Operator == "==" || Operator == "!=" || Operator == ">=" || Operator == "<=" || Operator == ">" || Operator == "<" || Operator == "&&" || Operator == "||")
        return Type::GetInt1();

    if (l->ParentOf(r))
        return l;
    if (r->ParentOf(l))
        return r;

    return nullptr; // Types are not the same
}

csaw::CallExpression::CallExpression(const SourceLoc& loc, const ExpressionPtr& callee, const std::vector<TypePtr>& temp_args, const std::vector<ExpressionPtr>& args)
    : Expression(loc), Callee(callee), TempArgs(temp_args), Args(args)
{
}

csaw::TypePtr csaw::CallExpression::GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args)
{
    TypePtr parent_type;
    if (const auto mem = std::dynamic_pointer_cast<MemberExpression>(Callee))
    {
        parent_type = mem->Object->GetType(symbols, false, {}, {});
        if (!parent_type) return nullptr;
    }

    std::vector<TypePtr> arg_types;
    for (const auto& arg : Args)
    {
        const auto arg_type = arg->GetType(symbols, false, {}, {});
        if (!arg_type) return nullptr;
        arg_types.push_back(arg_type);
    }
    const auto callee_type = Callee->GetType(symbols, true, parent_type, arg_types);
    if (!callee_type || !callee_type->IsFunctionPointer()) return nullptr;
    const auto fun_type = callee_type->AsPointer()->Base->AsFunction();
    if (!fun_type->Parent && !fun_type->Result)
        return Type::Get(std::dynamic_pointer_cast<IdentifierExpression>(Callee)->Id);
    return fun_type->Result;
}

csaw::CastExpression::CastExpression(const SourceLoc& loc, const TypePtr& type, const ExpressionPtr& value)
    : Expression(loc), Type(type), Value(value)
{
}

csaw::TypePtr csaw::CastExpression::GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args)
{
    return Type;
}

csaw::CharExpression::CharExpression(const SourceLoc& loc, const char value)
    : Expression(loc), Value(value)
{
}

csaw::CharExpression::CharExpression(const SourceLoc& loc, const std::string& value)
    : Expression(loc), Value(value[0])
{
}

csaw::TypePtr csaw::CharExpression::GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args)
{
    return Type::GetInt8();
}

csaw::DereferenceExpression::DereferenceExpression(const SourceLoc& loc, const ExpressionPtr& value)
    : Expression(loc), Value(value)
{
}

csaw::TypePtr csaw::DereferenceExpression::GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args)
{
    const auto v = Value->GetType(symbols, false, {}, {});
    if (!v || !v->IsPointer()) return nullptr;
    return v->AsPointer()->Base;
}

csaw::FloatExpression::FloatExpression(const SourceLoc& loc, const std::string& value)
    : Expression(loc), Value(std::stod(value))
{
}

csaw::TypePtr csaw::FloatExpression::GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args)
{
    return Type::GetFlt64();
}

csaw::IdentifierExpression::IdentifierExpression(const SourceLoc& loc, const std::string& id)
    : Expression(loc), Id(id)
{
}

csaw::TypePtr csaw::IdentifierExpression::GetType(std::map<std::string, std::vector<TypePtr>>& symbols, const bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args)
{
    if (symbols[Id].empty())
        return nullptr;

    if (const auto& symbol = symbols[Id].back(); !fun_only && symbol)
        return symbol;

    int highscore = 0;
    TypePtr best;

    for (const auto& symbol : symbols[Id])
    {
        if (symbol->IsFunctionPointer())
        {
            int score = 1;

            const auto fun = symbol->AsPointer()->Base->AsFunction();
            if (fun->Parent != parent) continue;
            if (fun->Args.size() > args.size() || (fun->Args.size() != args.size() && !fun->IsVararg)) continue;

            if (fun->Args.size() == args.size())
                score += 100;

            size_t i = 0;
            for (; i < fun->Args.size(); ++i)
            {
                if (fun->Args[i] == args[i])
                {
                    score += 10;
                    continue;
                }
                if (fun->Args[i]->ParentOf(args[i]))
                {
                    continue;
                }
                break;
            }
            if (i < fun->Args.size()) continue;

            if (score >= highscore)
            {
                highscore = score;
                best = symbol;
            }
        }
    }

    return best;
}

csaw::IndexExpression::IndexExpression(const SourceLoc& loc, const ExpressionPtr& array, const ExpressionPtr& index)
    : Expression(loc), Array(array), Index(index)
{
}

csaw::TypePtr csaw::IndexExpression::GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args)
{
    const auto a = Array->GetType(symbols, false, {}, {});
    const auto i = Index->GetType(symbols, false, {}, {});

    if (!a || !i) return nullptr;

    if (auto type = IdentifierExpression(Loc, "[]").GetType(symbols, true, a, {i}))
    {
        if (!type->IsFunctionPointer()) return nullptr;
        type = type->AsPointer()->Base->AsFunction()->Result;
        if (!type->IsPointer()) return nullptr;
        return type->AsPointer()->Base;
    }

    if (a->IsPointer())
        return a->AsPointer()->Base;

    if (a->IsArray())
        return a->AsArray()->Base;

    return nullptr;
}

csaw::IntExpression::IntExpression(const SourceLoc& loc, const uint64_t value)
    : Expression(loc), Value(value)
{
}

csaw::TypePtr csaw::IntExpression::GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args)
{
    return Type::GetInt64();
}

csaw::MemberExpression::MemberExpression(const SourceLoc& loc, const ExpressionPtr& object, const std::string& member, const bool deref)
    : Expression(loc), Object(object), Member(member), ShouldDeref(deref)
{
}

csaw::TypePtr csaw::MemberExpression::GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args)
{
    auto object_type = Object->GetType(symbols, false, {}, {});
    if (!object_type) return nullptr;
    if (ShouldDeref)
    {
        if (!object_type->IsPointer()) return nullptr;
        object_type = object_type->AsPointer()->Base;
    }

    auto member_type = object_type->IsStruct() ? object_type->AsStruct()->GetElement(Member).second : nullptr;

    if (!member_type)
    {
        member_type = IdentifierExpression(Loc, Member).GetType(symbols, true, object_type, args);
        if (!member_type || !member_type->IsFunctionPointer()) return nullptr;
        return member_type;
    }

    return member_type;
}

csaw::ReferenceExpression::ReferenceExpression(const SourceLoc& loc, const ExpressionPtr& value)
    : Expression(loc), Value(value)
{
}

csaw::TypePtr csaw::ReferenceExpression::GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args)
{
    const auto v = Value->GetType(symbols, false, {}, {});
    if (!v) return nullptr;
    return PointerType::Get(v);
}

csaw::SelectExpression::SelectExpression(const SourceLoc& loc, const ExpressionPtr& condition, const ExpressionPtr& _true, const ExpressionPtr& _false)
    : Expression(loc), Condition(condition), True(_true), False(_false)
{
}

csaw::TypePtr csaw::SelectExpression::GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args)
{
    auto t = True->GetType(symbols, fun_only, parent, args);
    auto f = False->GetType(symbols, fun_only, parent, args);

    if (!t || !f) return nullptr;

    if (t->ParentOf(f))
        return t;

    if (f->ParentOf(t))
        return f;

    return nullptr; // Types are not the same
}

csaw::SizeOfExpression::SizeOfExpression(const SourceLoc& loc, const TypePtr& type)
    : Expression(loc), Type(type)
{
}

csaw::TypePtr csaw::SizeOfExpression::GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args)
{
    return Type::GetInt64();
}

csaw::StringExpression::StringExpression(const SourceLoc& loc, const std::string& value)
    : Expression(loc), Value(value)
{
}

csaw::TypePtr csaw::StringExpression::GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args)
{
    return PointerType::Get(Type::GetInt8());
}

csaw::UnaryExpression::UnaryExpression(const SourceLoc& loc, const std::string& op, const ExpressionPtr& value, const bool rightop)
    : Expression(loc), Operator(op), Value(value), OpRight(rightop)
{
}

csaw::TypePtr csaw::UnaryExpression::GetType(std::map<std::string, std::vector<TypePtr>>& symbols, bool fun_only, const TypePtr& parent, const std::vector<TypePtr>& args)
{
    auto v = Value->GetType(symbols, false, {}, {});

    if (!v) return nullptr;

    if (OpRight)
    {
        if (const auto type = IdentifierExpression(Loc, Operator).GetType(symbols, true, v, {Type::GetInt1()}))
        {
            if (!type->IsFunctionPointer()) return nullptr;
            return type->AsPointer()->Base->AsFunction()->Result;
        }
    }
    else
    {
        if (const auto type = IdentifierExpression(Loc, Operator).GetType(symbols, true, v, {}))
        {
            if (!type->IsFunctionPointer()) return nullptr;
            return type->AsPointer()->Base->AsFunction()->Result;
        }
    }

    if (Operator == "!")
        return Type::GetInt1();

    return v;
}
