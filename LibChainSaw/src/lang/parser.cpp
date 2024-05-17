#include <iostream>
#include <csaw/CSaw.hpp>
#include <csaw/lang/Parser.hpp>

void csaw::Parser::Parse(const std::string& filename, std::istream& stream, const ParseCallback& callback, const std::vector<std::string>& includePaths)
{
    Parser parser(filename, stream, callback, includePaths);
    parser.Next();

    do
    {
        try { callback(parser.ParseStatement()); }
        catch (const ChainSawMessage& error)
        {
            const auto file = error.SourceFile.empty() ? filename : error.SourceFile;
            const auto line = error.SourceLine == 0 ? parser.m_Line : error.SourceLine;
            std::cerr << file << "(" << line << "): " << error.Message << std::endl;
            if (!error.CanRecover) throw;
        }
    }
    while (!parser.AtEOF());
}

int csaw::Parser::Escape() const
{
    int c = m_Stream.get();
    switch (c)
    {
    case 'a': return '\a';
    case 'b': return '\b';
    case 'f': return '\f';
    case 'n': return '\n';
    case 'r': return '\r';
    case 't': return '\t';
    case 'v': return '\v';
    default: break;
    }

    if (isdigit(c)) // nnn (oct)
    {
        std::string value;
        value += static_cast<char>(c);
        c = m_Stream.get();
        value += static_cast<char>(c);
        c = m_Stream.get();
        value += static_cast<char>(c);
        return std::stoi(value, nullptr, 8);
    }

    if (c == 'x') // hh (hex)
    {
        std::string value;
        c = m_Stream.get();
        value += static_cast<char>(c);
        c = m_Stream.get();
        value += static_cast<char>(c);
        return std::stoi(value, nullptr, 16);
    }

    return c;
}

csaw::Parser::Parser(const std::string& filename, std::istream& stream, const ParseCallback& callback, const std::vector<std::string>& includePaths)
    : m_Filename(filename), m_Stream(stream), m_Callback(callback), m_IncludePaths(includePaths)
{
}

bool csaw::Parser::AtEOF() const
{
    return m_Token.Type == TK_EOF;
}

bool csaw::Parser::At(const int type) const
{
    return m_Token.Type == type || m_Token.Type & type;
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

csaw::Token csaw::Parser::Expect(const int type)
{
    if (!At(type))
        CSAW_MESSAGE_(false, m_Filename, m_Line, "unexpected type " + std::string(ToString(m_Token.Type)) + ", expected " + ToString(type));
    Token token = m_Token;
    Next();
    return token;
}

void csaw::Parser::Expect(const std::string& value)
{
    if (!At(value))
        CSAW_MESSAGE_(false, m_Filename, m_Line, "unexpected value '" + m_Token.Value + "', expected '" + value + "'");
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
