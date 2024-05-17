#include <iostream>
#include <csaw/CSaw.hpp>
#include <csaw/lang/Parser.hpp>

static bool error(const csaw::ParseData& data, const csaw::ChainSawMessage& message, const size_t line)
{
    std::cerr << (message.Filename.empty() ? data.Filename : message.Filename) << "(" << (message.Line == 0 ? line : message.Line) << "): " << message.Message << std::endl;
    return !message.CanRecover;
}

void csaw::Parser::Parse(const ParseData& data)
{
    if (std::ranges::find(data.Processed, data.Filename) != data.Processed.end())
        return;

    data.Processed.emplace_back(data.Filename);
    Parser parser(data);

    do
    {
        try
        {
            while (parser.At(TK_COMPILE_DIRECTIVE)) parser.ParseCompileDirective();
            if (!parser.AtEOF())
                data.Callback(parser.ParseStatement());
        }
        catch (const ChainSawMessage& message)
        {
            if (error(data, message, parser.m_Line))
                break;
        }
    }
    while (!parser.AtEOF());
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
        CSAW_MESSAGE_(false, m_Data.Filename, m_Line, "Unexpected type " + TkToString(m_Token.Type) + ", expected " + TkToString(type));
    Token token = m_Token;
    Next();
    return token;
}

void csaw::Parser::Expect(const std::string& value)
{
    if (!At(value))
        CSAW_MESSAGE_(false, m_Data.Filename, m_Line, "Unexpected value '" + m_Token.Value + "', expected '" + value + "'");
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
