#include <iostream>
#include <csaw/Parser.hpp>

#define STATE_NORMAL 0
#define STATE_DETERMINE_COMMENT_TYPE 1
#define STATE_SINGLE_LINE_COMMENT 2
#define STATE_MULTI_LINE_COMMENT 3
#define STATE_STRING 4
#define STATE_CHAR 5
#define STATE_ID 6
#define STATE_NON_DEC 7
#define STATE_HEX 8
#define STATE_BIN 9
#define STATE_OCT 10
#define STATE_DEC 11
#define STATE_COMPILE_DIRECTIVE 12

#define isid(c) (isalnum(c) || c == '_')
#define isoct(c) ('0' <= c && c <= '7')

int csaw::Parser::Read()
{
    ++m_Loc.Column;
    return m_c = m_Data.Stream.get();
}

csaw::Token& csaw::Parser::Next()
{
    int state = STATE_NORMAL;

    auto loc = m_Loc;
    std::string value;
    bool is_flt = false;

    while (m_c != EOF)
    {
        switch (state)
        {
        case STATE_NORMAL:
            if (m_c <= 0x20)
            {
                if (m_c == '\n')
                {
                    m_Loc.Column = 0;
                    ++m_Loc.Row;
                }
                break;
            }
            if (m_c == '0')
            {
                loc = m_Loc;
                value += static_cast<char>(m_c);
                state = STATE_NON_DEC;
                break;
            }
            if (isdigit(m_c))
            {
                loc = m_Loc;
                value += static_cast<char>(m_c);
                state = STATE_DEC;
                break;
            }
            if (isid(m_c))
            {
                loc = m_Loc;
                value += static_cast<char>(m_c);
                state = STATE_ID;
                break;
            }
            switch (m_c)
            {
            case '#':
                state = STATE_DETERMINE_COMMENT_TYPE;
                break;

            case '"':
                loc = m_Loc;
                state = STATE_STRING;
                break;

            case '\'':
                loc = m_Loc;
                state = STATE_CHAR;
                break;

            case '\\':
                loc = m_Loc;
                state = STATE_COMPILE_DIRECTIVE;
                break;

            default:
                loc = m_Loc;
                value += static_cast<char>(m_c);
                Read();
                return m_Token = {loc, TK_OPERATOR, value};
            }
            break;

        case STATE_DETERMINE_COMMENT_TYPE:
            if (m_c == '#')
            {
                state = STATE_SINGLE_LINE_COMMENT;
            }
            else
            {
                state = STATE_MULTI_LINE_COMMENT;
            }
            break;

        case STATE_SINGLE_LINE_COMMENT:
            if (m_c == '\n')
            {
                m_Loc.Column = 0;
                ++m_Loc.Row;
                state = STATE_NORMAL;
            }
            break;

        case STATE_MULTI_LINE_COMMENT:
            if (m_c == '\n')
            {
                m_Loc.Column = 0;
                ++m_Loc.Row;
            }
            else if (m_c == '#')
            {
                state = STATE_NORMAL;
            }
            break;

        case STATE_STRING:
            if (m_c == '"')
            {
                Read();
                return m_Token = {loc, TK_STRING, value};
            }
            if (m_c == '\n')
            {
                m_Loc.Column = 0;
                ++m_Loc.Row;
                break;
            }
            if (m_c == '\\')
            {
                Read();
                switch (m_c)
                {
                case 'a':
                    m_c = '\a';
                    break;
                case 'b':
                    m_c = '\b';
                    break;
                case 'f':
                    m_c = '\f';
                    break;
                case 'n':
                    m_c = '\n';
                    break;
                case 'r':
                    m_c = '\r';
                    break;
                case 't':
                    m_c = '\t';
                    break;
                case 'v':
                    m_c = '\v';
                    break;
                }
            }
            value += static_cast<char>(m_c);
            break;

        case STATE_CHAR:
            if (m_c == '\'')
            {
                Read();
                return m_Token = {loc, TK_CHAR, value};
            }
            if (m_c == '\n')
            {
                m_Loc.Column = 0;
                ++m_Loc.Row;
                break;
            }
            if (m_c == '\\')
            {
                Read();
                switch (m_c)
                {
                case 'a':
                    m_c = '\a';
                    break;
                case 'b':
                    m_c = '\b';
                    break;
                case 'f':
                    m_c = '\f';
                    break;
                case 'n':
                    m_c = '\n';
                    break;
                case 'r':
                    m_c = '\r';
                    break;
                case 't':
                    m_c = '\t';
                    break;
                case 'v':
                    m_c = '\v';
                    break;
                }
            }
            value += static_cast<char>(m_c);
            break;

        case STATE_ID:
            if (!isid(m_c))
            {
                return m_Token = {loc, TK_IDENTIFIER, value};
            }
            value += static_cast<char>(m_c);
            break;

        case STATE_NON_DEC:
            if (m_c == 'x' || m_c == 'X')
            {
                value.clear();
                state = STATE_HEX;
            }
            else if (m_c == 'b' || m_c == 'B')
            {
                value.clear();
                state = STATE_BIN;
            }
            else if (isoct(m_c))
            {
                value += static_cast<char>(m_c);
                state = STATE_OCT;
            }
            else if (m_c == '.')
            {
                value += static_cast<char>(m_c);
                is_flt = true;
                state = STATE_DEC;
            }
            else if (isdigit(m_c))
            {
                value += static_cast<char>(m_c);
                state = STATE_DEC;
            }
            else
            {
                return m_Token = {loc, TK_INT_DEC, value};
            }
            break;

        case STATE_HEX:
            if (!isxdigit(m_c))
            {
                return m_Token = {loc, TK_INT_HEX, value};
            }
            value += static_cast<char>(m_c);
            break;

        case STATE_BIN:
            if (m_c != '0' && m_c != '1')
            {
                return m_Token = {loc, TK_INT_BIN, value};
            }
            value += static_cast<char>(m_c);
            break;

        case STATE_OCT:
            if (!isoct(m_c))
            {
                return m_Token = {loc, TK_INT_OCT, value};
            }
            value += static_cast<char>(m_c);
            break;

        case STATE_DEC:
            if (m_c == 'e' || m_c == 'E')
            {
                value += static_cast<char>(m_c);
                Read();
                if (m_c == '-' || m_c == '+')
                {
                    value += static_cast<char>(m_c);
                    Read();
                }
            }
            if (m_c == '.')
            {
                is_flt = true;
                value += static_cast<char>(m_c);
                Read();
            }
            if (!isdigit(m_c))
            {
                return m_Token = {loc, is_flt ? TK_FLOAT : TK_INT_DEC, value};
            }
            value += static_cast<char>(m_c);
            break;

        case STATE_COMPILE_DIRECTIVE:
            if (!isid(m_c))
                return m_Token = {loc, TK_COMPILE_DIRECTIVE, value};
            value += static_cast<char>(m_c);
            break;
        }

        Read();
    }

    return m_Token = {loc, TK_EOF};
}
