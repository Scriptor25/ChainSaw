#include <csaw/codegen/Value.hpp>

csaw::Value::Value(const std::string &name, const csaw::TypePtr &type)
        : Name(name), Type(type)
{
}

csaw::Value::~Value() noexcept = default;

csaw::NumValue::NumValue(const std::string &name, const csaw::TypePtr &type, const double x)
        : Value(name, type), X(x)
{
}

csaw::Arg::Arg(const std::string &name, const csaw::TypePtr &type)
        : Value(name, type)
{
}
