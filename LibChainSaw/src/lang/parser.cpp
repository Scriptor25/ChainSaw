#include <iostream>
#include <csaw/Error.hpp>
#include <csaw/Parser.hpp>

int csaw::Parser::Parse(const ParseData& data)
{
    if (std::ranges::find(data.Processed, data.Filename) != data.Processed.end())
        return 0;

    data.Processed.emplace_back(data.Filename);
    Parser parser(data);

    CSawError = 0;

    try
    {
        do
        {
            while (parser.At(TK_COMPILE_DIRECTIVE)) parser.ParseCompileDirective();
            if (!parser.AtEOF())
                data.Callback(parser.ParseStatement());
        }
        while (!parser.AtEOF());
    }
    catch (const std::runtime_error& error)
    {
        std::cout << data.Filename << ": " << error.what() << std::endl;
        return 1;
    }

    return CSawError;
}

csaw::Parser::Parser(const ParseData& data)
    : m_Data(data), m_Loc{data.Filename, 1, 1}
{
    Next();
}

int csaw::Parser::Escape()
{
    int c = m_Data.Stream.get();
    ++m_Loc.Column;

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
        c = m_Data.Stream.get();
        ++m_Loc.Column;
        value += static_cast<char>(c);
        c = m_Data.Stream.get();
        ++m_Loc.Column;
        value += static_cast<char>(c);
        return std::stoi(value, nullptr, 8);
    }

    if (c == 'x') // hh (hex)
    {
        std::string value;
        c = m_Data.Stream.get();
        ++m_Loc.Column;
        value += static_cast<char>(c);
        c = m_Data.Stream.get();
        ++m_Loc.Column;
        value += static_cast<char>(c);
        return std::stoi(value, nullptr, 16);
    }

    return c;
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
        ThrowError(m_Loc, true, "Expected type %s, found type %s, value '%s'", TkToString(type).c_str(), TkToString(m_Token.Type).c_str(), m_Token.Value.c_str());
    Token token = m_Token;
    Next();
    return token;
}

void csaw::Parser::Expect(const std::string& value)
{
    if (!At(value))
        ThrowError(m_Loc, true, "Expected value '%s', found type %s, value '%s'", value.c_str(), TkToString(m_Token.Type).c_str(), m_Token.Value.c_str());
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
