#include <stdexcept>
#include <csaw/lang/Parser.hpp>

void csaw::Parser::Parse(std::istream& stream, const ParseCallback& callback, const std::vector<std::string>& includePaths)
{
    Parser parser(stream, callback, includePaths);

    parser.Next();
    do callback(parser.ParseStatement());
    while (!parser.AtEOF());
}

int csaw::Parser::Escape(const int c)
{
    switch (c)
    {
    case 'a': return '\a';
    case 'b': return '\b';
    case 'f': return '\f';
    case 'n': return '\n';
    case 'r': return '\r';
    case 't': return '\t';
    case 'v': return '\v';
    default: return c;
    }
}

csaw::Parser::Parser(std::istream& stream, const ParseCallback& callback, const std::vector<std::string>& includePaths)
    : m_Stream(stream), m_Callback(callback), m_IncludePaths(includePaths)
{
}

bool csaw::Parser::AtEOF() const
{
    return m_Token.Type == TK_EOF;
}

bool csaw::Parser::At(const TokenType type) const
{
    return m_Token.Type == type;
}

bool csaw::Parser::At(const std::string& value) const
{
    return m_Token.Value == value;
}

csaw::Token csaw::Parser::Get()
{
    Token token = m_Token;
    Next();
    return token;
}

csaw::Token csaw::Parser::Expect(const TokenType type)
{
    if (!At(type))
        throw std::runtime_error("unexpected type");
    Token token = m_Token;
    Next();
    return token;
}

void csaw::Parser::Expect(const std::string& value)
{
    if (!At(value))
        throw std::runtime_error("unexpected value");
    Next();
}

bool csaw::Parser::NextIfAt(const std::string& value)
{
    if (At(value))
    {
        Next();
        return true;
    }
    return false;
}
