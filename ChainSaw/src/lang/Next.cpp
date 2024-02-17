#include <lang/Parser.h>

csaw::lang::Token& csaw::lang::Parser::Next()
{
	auto c = Read();

	while (c != EOF && c <= 0x20)
	{
		if (c == '\n')
			m_Line++;
		c = Read();
	}

	if (c == EOF)
		return m_Token = Token(m_Line);

	if (c == '#')
	{
		c = Read();
		char delim;
		if (c == '#')
			delim = '\n';
		else
			delim = '#';

		while (c != EOF && c != delim)
		{
			if (c == '\n')
				m_Line++;
			c = Read();
		}

		if (c == '\n')
			m_Line++;

		return Next();
	}

	if (c == '"')
	{
		std::string str;

		c = Read();
		while (c != EOF && c != '"')
		{
			if (c == '\\')
				c = Escape(Read());

			str += c;
			c = Read();
		}

		return m_Token = Token(TK_STRING, str, m_Line);
	}

	if (c == '\'')
	{
		std::string str;

		c = Read();
		while (c != EOF && c != '"')
		{
			if (c == '\\')
				c = Escape(Read());

			str += c;
			c = Read();
		}

		return m_Token = Token(TK_CHAR, str, m_Line);
	}

	if (c == '0')
	{
		Mark();
		int p = Read();

		int mode = 0;
		if (p == 'x' || p == 'X')
			mode = 1;
		else if (p == 'b' || p == 'B')
			mode = 2;

		if (mode == 0)
			Reset();
		else
		{
			std::string str;

			c = Read();
			do {
				str += c;
				Mark();
				c = Read();
			} while (isxdigit(c));
			Reset();

			return m_Token = Token(mode == 1 ? TK_HEX : TK_BIN, str, m_Line);
		}
	}

	if (isdigit(c))
	{
		std::string str;

		do {
			str += c;
			Mark();
			c = Read();

			if (c == 'e' || c == 'E')
			{
				str += c;
				c = Read();
				if (c == '-')
					str += c;
				Mark();
				c = Read();
			}

		} while (isdigit(c) || c == '.');
		Reset();

		return m_Token = Token(TK_DEC, str, m_Line);
	}

	if (isalpha(c) || c == '_')
	{
		std::string str;

		do {
			str += c;
			Mark();
			c = Read();
		} while (isalnum(c) || c == '_');
		Reset();

		return m_Token = Token(TK_IDENTIFIER, str, m_Line);
	}

	return m_Token = Token(TK_OPERATOR, c, m_Line);
}
