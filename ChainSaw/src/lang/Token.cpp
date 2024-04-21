#include <lang/Parser.hpp>

csaw::lang::Token::Token(const TokenType type, const std::string &value, const size_t line)
        : Type(type), Value(value), Line(line)
{
}

csaw::lang::Token::Token(const TokenType type, const int value, const size_t line)
        : Token(type, std::string(1, value), line)
{
}

csaw::lang::Token::Token(const size_t line)
        : Token(TK_EOF, std::string(), line)
{
}

csaw::lang::Token::Token()
        : Token(TK_, std::string(), 0)
{
}

std::ostream &csaw::lang::operator<<(std::ostream &out, const Token &token)
{
    return out << token.Value << "[" << token.Type << "](" << token.Line << ")";
}

std::ostream &csaw::lang::operator<<(std::ostream &out, const TokenType &type)
{
    switch (type)
    {
        case TK_EOF: return out << "EOF";
        case TK_IDENTIFIER: return out << "IDENTIFIER";
        case TK_DEC: return out << "DEC";
        case TK_HEX: return out << "HEX";
        case TK_BIN: return out << "BIN";
        case TK_STRING: return out << "STRING";
        case TK_CHAR: return out << "CHAR";
        case TK_OPERATOR: return out << "OPERATOR";

        default:
        case TK_: return out << "UNDEFINED";
    }
}
