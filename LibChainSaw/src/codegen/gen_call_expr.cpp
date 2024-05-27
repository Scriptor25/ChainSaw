#include <csaw/Builder.hpp>
#include <csaw/Error.hpp>
#include <csaw/Expr.hpp>
#include <csaw/Signature.hpp>
#include <csaw/Type.hpp>
#include <csaw/Value.hpp>

csaw::RValuePtr csaw::Builder::Gen(const CallExpression& expression)
{
    std::string name;
    LValuePtr lobject;

    if (const auto identifier_expression = std::dynamic_pointer_cast<IdentifierExpression>(expression.Callee))
    {
        name = identifier_expression->Id;
    }
    else if (const auto member_expression = std::dynamic_pointer_cast<MemberExpression>(expression.Callee))
    {
        name = member_expression->Member;
        const auto object = Gen(member_expression->Object);
        if (!object)
            return nullptr;

        if (object->IsLValue())
        {
            lobject = std::dynamic_pointer_cast<LValue>(object);
            if (member_expression->ShouldDeref)
            {
                const auto deref = lobject->Dereference();
                if (!deref)
                {
                    ThrowErrorStmt(expression, false, "Failed to dereference: %s", deref.Msg().c_str());
                    return nullptr;
                }
                lobject = deref.Get();
            }
        }
        else
        {
            if (member_expression->ShouldDeref)
            {
                const auto deref = object->GetRValue()->Dereference(this);
                if (!deref)
                {
                    ThrowErrorStmt(expression, false, "Failed to dereference: %s", deref.Msg().c_str());
                    return nullptr;
                }
                lobject = deref.Get();
            }
            else
            {
                const auto alloc = LValue::AllocateAndStore(this, object->GetType(), object->GetValue());
                if (!alloc)
                {
                    ThrowErrorStmt(expression, false, "Failed to allocate: %s", alloc.Msg().c_str());
                    return nullptr;
                }
                lobject = alloc.Get();
            }
        }
    }
    else throw;

    std::vector<ValuePtr> args;
    std::vector<TypePtr> arg_types;

    for (const auto& arg : expression.Args)
    {
        const auto value = Gen(arg);
        if (!value)
            return nullptr;

        args.push_back(value);
        arg_types.push_back(value->GetType());
    }

    const auto [function, signature] = FindFunction(name, lobject ? lobject->GetType() : nullptr, arg_types);
    if (!function)
    {
        std::string sig_name = name;
        if (lobject)
            sig_name += ':' + lobject->GetType()->Name;

        std::string args_string;
        for (size_t i = 0; i < arg_types.size(); ++i)
        {
            if (i > 0) args_string += ", ";
            args_string += arg_types[i]->Name;
        }

        ThrowErrorStmt(expression, false, "Failed to find function '@%s(%s)'", sig_name.c_str(), args_string.c_str());
        return nullptr;
    }

    std::vector<llvm::Value*> vargs;
    LValuePtr lresult;
    if (signature.IsConstructor())
    {
        const auto alloc = LValue::Allocate(this, Type::Get(signature.Name));
        if (!alloc)
        {
            ThrowErrorStmt(expression, false, "Failed to allocate: %s", alloc.Msg().c_str());
            return nullptr;
        }

        lresult = alloc.Get();
        vargs.push_back(lresult->GetPointer());
    }

    if (lobject)
        vargs.push_back(lobject->GetPointer());

    size_t i = 0;
    for (; i < signature.Args.size(); ++i)
    {
        const auto cast = Cast(args[i], signature.Args[i]);
        if (!cast)
        {
            ThrowErrorStmt(expression, false, "Failed to cast: %s", cast.Msg().c_str());
            return nullptr;
        }

        vargs.push_back(cast.Get()->GetValue());
    }
    for (; i < args.size(); ++i)
        vargs.push_back(args[i]->GetValue());

    const auto result = GetBuilder().CreateCall(function->getFunctionType(), function, vargs);

    if (signature.IsConstructor())
        return lresult->GetRValue();

    return RValue::Create(signature.Result, result);
}
