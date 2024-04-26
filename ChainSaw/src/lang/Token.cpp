#include <csaw/lang/Parser.hpp>

csaw::Token::Token(const TokenType type, const std::string &value, const size_t line)
        : Type(type), Value(value), Line(line)
{
}

csaw::Token::Token(const TokenType type, const int value, const size_t line)
        : Token(type, std::string(1, value), line)
{
}

csaw::Token::Token(const size_t line)
        : Token(TK_EOF, std::string(), line)
{
}

csaw::Token::Token()
        : Token(TK_EOF, std::string(), 0)
{
}

std::ostream &csaw::operator<<(std::ostream &out, const TokenType &type)
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
        case TK_COMP_DIR: return out << "COMP DIR";
        default: return out << "UNDEFINED";
    }
}

std::ostream &csaw::operator<<(std::ostream &out, const Token &token)
{
    return out << token.Value << "[" << token.Type << "](" << token.Line << ")";
}
