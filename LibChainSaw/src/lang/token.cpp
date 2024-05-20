#include <ostream>
#include <string>
#include <csaw/lang/Parser.hpp>

uint64_t csaw::Token::IntValue() const
{
    switch (Type)
    {
    case TK_INT_BIN: return std::stoull(Value, nullptr, 2);
    case TK_INT_OCT: return std::stoull(Value, nullptr, 8);
    case TK_INT_DEC: return std::stoull(Value, nullptr, 10);
    case TK_INT_HEX: return std::stoull(Value, nullptr, 16);
    default: return 0;
    }
}

std::string csaw::TkToString(const int type)
{
    if (type == TK_EOF) return "EOF";

    std::vector<std::string> strs;
    if (type & TK_IDENTIFIER) strs.emplace_back("IDENTIFIER");
    if (type & TK_INT_BIN) strs.emplace_back("INT_BIN");
    if (type & TK_INT_OCT) strs.emplace_back("INT_OCT");
    if (type & TK_INT_DEC) strs.emplace_back("INT_DEC");
    if (type & TK_INT_HEX) strs.emplace_back("INT_HEX");
    if (type & TK_FLOAT) strs.emplace_back("FLOAT");
    if (type & TK_STRING) strs.emplace_back("STRING");
    if (type & TK_CHAR) strs.emplace_back("CHAR");
    if (type & TK_OPERATOR) strs.emplace_back("OPERATOR");
    if (type & TK_COMPILE_DIRECTIVE) strs.emplace_back("COMPILE_DIRECTIVE");

    if (strs.empty())
        return "UNKNOWN";

    std::string str;
    for (size_t i = 0; i < strs.size(); ++i)
    {
        if (i > 0) str += '|';
        str += strs[i];
    }
    return str;
}

std::ostream& csaw::operator<<(std::ostream& out, const TokenType& type)
{
    return out << TkToString(type);
}

std::ostream& csaw::operator<<(std::ostream& out, const Token& token)
{
    return out << token.Line << ": " << token.Value << " (" << token.Type << ')';
}
