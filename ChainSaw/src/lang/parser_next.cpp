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
        std::string value;

        c = m_Stream.get();
        while (c != EOF && c != '"')
        {
            if (c == '\\')
                c = Escape(m_Stream.get());

            value += static_cast<char>(c);
            c = m_Stream.get();
        }

        return m_Token = Token(TK_STRING, value, m_Line);
    }

    if (c == '\'')
    {
        std::string value;

        c = m_Stream.get();
        while (c != EOF && c != '"')
        {
            if (c == '\\')
                c = Escape(m_Stream.get());

            value += static_cast<char>(c);
            c = m_Stream.get();
        }

        return m_Token = Token(TK_CHAR, value, m_Line);
    }

    if (c == '0')
    {
        const int p = m_Stream.get();

        int mode = 0;
        if (p == 'x' || p == 'X')
            mode = 1;
        else if (p == 'b' || p == 'B')
            mode = 2;
        else if (isdigit(p))
        {
            mode = 3;
            m_Stream.putback(static_cast<char>(p));
        }
        else m_Stream.putback(static_cast<char>(p));

        if (mode)
        {
            std::string value;

            c = m_Stream.get();
            do
            {
                value += static_cast<char>(c);
                c = m_Stream.get();
            }
            while (isxdigit(c));
            m_Stream.putback(static_cast<char>(c));

            return m_Token = Token(mode == 1 ? TK_INT_HEX : mode == 2 ? TK_INT_BIN : TK_INT_OCT, value, m_Line);
        }
    }

    if (isdigit(c))
    {
        std::string value;
        bool period = false;

        do
        {
            value += static_cast<char>(c);
            c = m_Stream.get();

            if (c == '.')
            {
                if (period)
                    throw std::runtime_error("a floating point number can only have one period");

                period = true;
                value += static_cast<char>(c);
                c = m_Stream.get();
            }

            if (c == 'e' || c == 'E')
            {
                value += static_cast<char>(c);
                c = m_Stream.get();
                if (c == '-')
                    value += static_cast<char>(c);
                c = m_Stream.get();
            }
        }
        while (isdigit(c));
        m_Stream.putback(static_cast<char>(c));

        return m_Token = Token(period ? TK_FLOAT : TK_INT_DEC, value, m_Line);
    }

    if (isalpha(c) || c == '_')
    {
        std::string value;

        do
        {
            value += static_cast<char>(c);
            c = m_Stream.get();
        }
        while (isalnum(c) || c == '_');
        m_Stream.putback(static_cast<char>(c));

        return m_Token = Token(TK_IDENTIFIER, value, m_Line);
    }

    return m_Token = Token(TK_OPERATOR, c, m_Line);
}
