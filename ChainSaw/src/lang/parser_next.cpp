#include <istream>
#include <csaw/lang/Parser.hpp>

csaw::Token& csaw::Parser::Next()
{
    auto c = m_Stream.get();

    while (c != EOF && c <= 0x20)
    {
        if (c == '\n')
            m_Line++;
        c = m_Stream.get();
    }

    if (c == EOF)
        return m_Token = Token(m_Line);

    if (c == '#')
    {
        c = m_Stream.get();
        char delim;
        if (c == '#')
            delim = '\n';
        else
            delim = '#';

        while (c != EOF && c != delim)
        {
            if (c == '\n')
                m_Line++;
            c = m_Stream.get();
        }

        if (c == '\n')
            m_Line++;

        return Next();
    }

    if (c == '\\')
    {
        c = m_Stream.get();

        std::string value;
        do
        {
            value += static_cast<char>(c);
            c = m_Stream.get();
        }
        while (isalnum(c) || c == '_');
        m_Stream.putback(static_cast<char>(c));

        return m_Token = Token(TK_COMPILE_DIRECTIVE, value, m_Line);
    }

    if (c == '"')
    {
        std::string str;

        c = m_Stream.get();
        while (c != EOF && c != '"')
        {
            if (c == '\\')
                c = Escape(m_Stream.get());

            str += static_cast<char>(c);
            c = m_Stream.get();
        }

        return m_Token = Token(TK_STRING, str, m_Line);
    }

    if (c == '\'')
    {
        std::string str;

        c = m_Stream.get();
        while (c != EOF && c != '"')
        {
            if (c == '\\')
                c = Escape(m_Stream.get());

            str += static_cast<char>(c);
            c = m_Stream.get();
        }

        return m_Token = Token(TK_CHAR, str, m_Line);
    }

    if (c == '0')
    {
        const int p = m_Stream.get();

        int mode = 0;
        if (p == 'x' || p == 'X')
            mode = 1;
        else if (p == 'b' || p == 'B')
            mode = 2;

        if (mode == 0)
            m_Stream.putback(static_cast<char>(p));
        else
        {
            std::string str;

            c = m_Stream.get();
            do
            {
                str += static_cast<char>(c);
                c = m_Stream.get();
            }
            while (isxdigit(c));
            m_Stream.putback(static_cast<char>(c));

            return m_Token = Token(mode == 1 ? TK_HEX : TK_BIN, str, m_Line);
        }
    }

    if (isdigit(c))
    {
        std::string str;

        do
        {
            str += static_cast<char>(c);
            c = m_Stream.get();

            if (c == 'e' || c == 'E')
            {
                str += static_cast<char>(c);
                c = m_Stream.get();
                if (c == '-')
                    str += static_cast<char>(c);
                c = m_Stream.get();
            }
        }
        while (isdigit(c) || c == '.');
        m_Stream.putback(static_cast<char>(c));

        return m_Token = Token(TK_DEC, str, m_Line);
    }

    if (isalpha(c) || c == '_')
    {
        std::string str;

        do
        {
            str += static_cast<char>(c);
            c = m_Stream.get();
        }
        while (isalnum(c) || c == '_');
        m_Stream.putback(static_cast<char>(c));

        return m_Token = Token(TK_IDENTIFIER, str, m_Line);
    }

    return m_Token = Token(TK_OPERATOR, c, m_Line);
}
