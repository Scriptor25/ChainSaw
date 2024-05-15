#pragma once

namespace csaw
{
    class Builder;

    class Value;
    class LValue;
    class RValue;
    typedef std::shared_ptr<Value> ValuePtr;
    typedef std::shared_ptr<LValue> LValuePtr;
    typedef std::shared_ptr<RValue> RValuePtr;

    struct FunctionRef;
}
