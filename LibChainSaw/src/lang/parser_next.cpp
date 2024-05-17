#include <istream>
#include <csaw/CSaw.hpp>
#include <csaw/lang/Parser.hpp>

csaw::Token& csaw::Parser::Next()
{
    auto c = m_Data.Stream.get();

    while (c != EOF && c <= 0x20)
    {
        if (c == '\n')
            m_Line++;
        c = m_Data.Stream.get();
    }

    if (c == EOF)
        return m_Token = {TK_EOF, {}, m_Line};

    if (c == '#')
    {
        c = m_Data.Stream.get();
        char delim;
        if (c == '#')
            delim = '\n';
        else
            delim = '#';

        while (c != EOF && c != delim)
        {
            if (c == '\n')
                m_Line++;
            c = m_Data.Stream.get();
        }

        if (c == '\n')
            m_Line++;

        return Next();
    }

    if (c == '\\')
    {
        c = m_Data.Stream.get();

        std::string value;
        do
        {
            value += static_cast<char>(c);
            c = m_Data.Stream.get();
        }
        while (isalnum(c) || c == '_');
        m_Data.Stream.putback(static_cast<char>(c));

        return m_Token = {TK_COMPILE_DIRECTIVE, value, m_Line};
    }

    if (c == '"')
    {
        std::string value;

        c = m_Data.Stream.get();
        while (c != EOF && c != '"')
        {
            if (c == '\\')
                c = Escape();

            value += static_cast<char>(c);
            c = m_Data.Stream.get();
        }

        return m_Token = {TK_STRING, value, m_Line};
    }

    if (c == '\'')
    {
        std::string value;

        c = m_Data.Stream.get();
        while (c != EOF && c != '\'')
        {
            if (c == '\\')
                c = Escape();

            value += static_cast<char>(c);
            c = m_Data.Stream.get();
        }

        return m_Token = {TK_CHAR, value, m_Line};
    }

    if (c == '0')
    {
        const int p = m_Data.Stream.get();

        int mode = 0;
        if (p == 'x' || p == 'X')
            mode = 1;
        else if (p == 'b' || p == 'B')
            mode = 2;
        else if (isdigit(p))
        {
            mode = 3;
            m_Data.Stream.putback(static_cast<char>(p));
        }
        else m_Data.Stream.putback(static_cast<char>(p));

        if (mode)
        {
            std::string value;

            c = m_Data.Stream.get();
            do
            {
                value += static_cast<char>(c);
                c = m_Data.Stream.get();
            }
            while (isxdigit(c));
            m_Data.Stream.putback(static_cast<char>(c));

            return m_Token = {mode == 1 ? TK_INT_HEX : mode == 2 ? TK_INT_BIN : TK_INT_OCT, value, m_Line};
        }
    }

    if (isdigit(c))
    {
        std::string value;
        bool flt = false;

        do
        {
            value += static_cast<char>(c);
            c = m_Data.Stream.get();

            if (c == '.')
            {
                if (flt)
                    CSAW_MESSAGE_(false, m_Data.Filename, m_Line, "Already a floating point number");

                flt = true;
                value += static_cast<char>(c);
                c = m_Data.Stream.get();
            }

            if (c == 'e' || c == 'E')
            {
                if (flt)
                    CSAW_MESSAGE_(false, m_Data.Filename, m_Line, "Already a floating point number");

                flt = true;
                value += static_cast<char>(c);
                c = m_Data.Stream.get();
                if (c == '-')
                    value += static_cast<char>(c);
                c = m_Data.Stream.get();
            }
        }
        while (isdigit(c));
        m_Data.Stream.putback(static_cast<char>(c));

        return m_Token = {flt ? TK_FLOAT : TK_INT_DEC, value, m_Line};
    }

    if (isalpha(c) || c == '_')
    {
        std::string value;

        do
        {
            value += static_cast<char>(c);
            c = m_Data.Stream.get();
        }
        while (isalnum(c) || c == '_');
        m_Data.Stream.putback(static_cast<char>(c));

        return m_Token = {TK_IDENTIFIER, value, m_Line};
    }

    return m_Token = {TK_OPERATOR, std::string(1, static_cast<char>(c)), m_Line};
}
