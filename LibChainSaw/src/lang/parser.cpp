#include <iostream>
#include <csaw/Error.hpp>
#include <csaw/lang/Parser.hpp>

void csaw::Parser::Parse(const ParseData& data)
{
    if (std::ranges::find(data.Processed, data.Filename) != data.Processed.end())
        return;

    data.Processed.emplace_back(data.Filename);
    Parser parser(data);

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
    }
}

csaw::Parser::Parser(const ParseData& data)
    : m_Data(data)
{
    Next();
}

int csaw::Parser::Escape() const
{
    int c = m_Data.Stream.get();
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
        value += static_cast<char>(c);
        c = m_Data.Stream.get();
        value += static_cast<char>(c);
        return std::stoi(value, nullptr, 8);
    }

    if (c == 'x') // hh (hex)
    {
        std::string value;
        c = m_Data.Stream.get();
        value += static_cast<char>(c);
        c = m_Data.Stream.get();
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
        ThrowError(m_Data.Filename, m_Line, true, "Expected type %s, found %s", TkToString(type).c_str(), TkToString(m_Token.Type).c_str());
    Token token = m_Token;
    Next();
    return token;
}

void csaw::Parser::Expect(const std::string& value)
{
    if (!At(value))
        ThrowError(m_Data.Filename, m_Line, true, "Expected value %s, found %s", value.c_str(), m_Token.Value.c_str());
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
