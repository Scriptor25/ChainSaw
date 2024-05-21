#include <istream>
#include <csaw/Error.hpp>
#include <csaw/Parser.hpp>

#define NEXT_NORMAL       0
#define NEXT_COMMENT_TYPE 1
#define NEXT_COMMENT      2
#define NEXT_LINE_COMMENT 3
#define NEXT_COMP_DIR     4
#define NEXT_STRING       5
#define NEXT_CHAR         6
#define NEXT_NON_DEC_TYPE 7
#define NEXT_HEX          8
#define NEXT_BIN          9
#define NEXT_OCT         10
#define NEXT_DEC         11
#define NEXT_IDENT       12

#define isid(c) (isalnum(c) || c == '_')

int csaw::Parser::Read()
{
    m_Col = m_Loc.Column;
    ++m_Loc.Column;
    return m_Data.Stream.get();
}

void csaw::Parser::PutBack(const int c)
{
    m_Data.Stream.putback(static_cast<char>(c));
    --m_Loc.Column;
    if (m_Loc.Column == 0)
    {
        m_Loc.Column = m_Col;
        --m_Loc.Line;
    }
}

csaw::Token& csaw::Parser::Next()
{
    int state = NEXT_NORMAL;

    std::string value;
    bool flt = false;

    auto loc = m_Loc;

    while (true)
    {
        int c = Read();
        if (c == EOF)
            return m_Token = {loc, TK_EOF, {}};

        switch (state)
        {
        case NEXT_NORMAL:
            switch (c)
            {
            case '\n':
                m_Col = m_Loc.Column;
                m_Loc.Column = 1;
                ++m_Loc.Line;
                continue;

            case '#':
                state = NEXT_COMMENT_TYPE;
                continue;

            case '\\':
                state = NEXT_COMP_DIR;
                continue;

            case '"':
                state = NEXT_STRING;
                continue;

            case '\'':
                state = NEXT_CHAR;
                continue;

            case '0':
                state = NEXT_NON_DEC_TYPE;
                continue;

            default:
                break;
            }

            if (c <= 0x20)
            {
                loc = m_Loc;
                continue;
            }

            if (isdigit(c))
            {
                state = NEXT_DEC;
                value += static_cast<char>(c);
                continue;
            }

            if (isid(c))
            {
                state = NEXT_IDENT;
                value += static_cast<char>(c);
                continue;
            }

            return m_Token = {loc, TK_OPERATOR, std::string(1, static_cast<char>(c))};

        case NEXT_COMMENT_TYPE:
            if (c == '#')
                state = NEXT_LINE_COMMENT;
            else
                state = NEXT_COMMENT;
            break;

        case NEXT_COMMENT:
            if (c == '\n')
            {
                m_Col = m_Loc.Column;
                m_Loc.Column = 1;
                ++m_Loc.Line;
            }
            if (c == '#')
                state = NEXT_NORMAL;
            break;

        case NEXT_LINE_COMMENT:
            if (c == '\n')
            {
                state = NEXT_NORMAL;
                m_Col = m_Loc.Column;
                m_Loc.Column = 1;
                ++m_Loc.Line;
            }
            break;

        case NEXT_COMP_DIR:
            if (!isid(c))
            {
                PutBack(c);
                return m_Token = {loc, TK_COMPILE_DIRECTIVE, value};
            }

            value += static_cast<char>(c);
            break;

        case NEXT_STRING:
            if (c == '"')
                return m_Token = {loc, TK_STRING, value};
            if (c == '\\')
                c = Escape();
            value += static_cast<char>(c);
            break;

        case NEXT_CHAR:
            if (c == '\'')
                return m_Token = {loc, TK_CHAR, value};
            if (c == '\\')
                c = Escape();
            value += static_cast<char>(c);
            break;

        case NEXT_NON_DEC_TYPE:
            if (c == 'x' || c == 'X')
                state = NEXT_HEX;
            else if (c == 'b' || c == 'B')
                state = NEXT_BIN;
            else if (isdigit(c))
                state = NEXT_OCT;
            else
            {
                PutBack(c);
                state = NEXT_DEC;
                value += '0';
            }
            break;

        case NEXT_BIN:
        case NEXT_OCT:
        case NEXT_HEX:
            if (!isxdigit(c))
            {
                PutBack(c);
                return m_Token = {loc, state == NEXT_BIN ? TK_INT_BIN : state == NEXT_OCT ? TK_INT_OCT : TK_INT_HEX, value};
            }
            value += static_cast<char>(c);
            break;

        case NEXT_DEC:
            if (c == '.')
            {
                if (flt)
                    ThrowError(m_Loc, true, "Value already has a floating point");
                flt = true;
                value += static_cast<char>(c);
                break;
            }

            if (c == 'e' || c == 'E')
            {
                if (flt)
                    ThrowError(m_Loc, true, "Value already has a floating point");
                flt = true;
                value += static_cast<char>(c);
                c = Read();
                if (c == '-' || c == '+')
                    value += static_cast<char>(c);
                else PutBack(c);
                break;
            }

            if (!isdigit(c))
            {
                PutBack(c);
                return m_Token = {loc, flt ? TK_FLOAT : TK_INT_DEC, value};
            }

            value += static_cast<char>(c);
            break;

        case NEXT_IDENT:
            if (!isid(c))
            {
                PutBack(c);
                return m_Token = {loc, TK_IDENTIFIER, value};
            }
            value += static_cast<char>(c);
            break;

        default:
            state = NEXT_NORMAL;
            break;
        }
    }
}
