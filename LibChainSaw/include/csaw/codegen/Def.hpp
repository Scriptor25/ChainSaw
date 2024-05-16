#pragma once

namespace csaw
{
    class Builder;
    struct Signature;

    class Value;
    class LValue;
    class RValue;
    typedef std::shared_ptr<Value> ValuePtr;
    typedef std::shared_ptr<LValue> LValuePtr;
    typedef std::shared_ptr<RValue> RValuePtr;
}
