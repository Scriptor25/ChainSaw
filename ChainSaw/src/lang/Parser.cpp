#include <lang/Parser.h>

#include <cassert>
#include <iostream>

void csaw::lang::Parser::Parse(std::istream& stream, std::function<void(StmtPtr stmt)> callback)
{
	Parser parser(stream);

	parser.Next();
	do {
		callback(parser.ParseStmt());
	} while (!parser.AtEOF());
}

csaw::lang::Parser::Parser(std::istream& stream)
	: m_Stream(stream)
{
}

csaw::lang::Parser::~Parser()
{
}

int csaw::lang::Parser::Read()
{
	int c = m_Stream.get();
	m_Buffered.push_back(c);
	return c;
}

void csaw::lang::Parser::Mark()
{
	m_Buffered.clear();
	m_Marked = true;
}

void csaw::lang::Parser::Reset()
{
	for (size_t i = 0; i < m_Buffered.size(); i++)
		m_Stream.putback(m_Buffered[m_Buffered.size() - 1 - i]);
	m_Buffered.clear();
	m_Marked = false;
}

int csaw::lang::Parser::Escape(int c)
{
	switch (c)
	{
	case 'a': return '\a';
	case 'b': return '\b';
	case 'f': return '\f';
	case 'n': return '\n';
	case 'r': return '\r';
	case 't': return '\t';
	case 'v': return '\v';
	default:  return c;
	}
}

bool csaw::lang::Parser::AtEOF() const
{
	return m_Token.Type == TK_EOF;
}

bool csaw::lang::Parser::At(const TokenType type) const
{
	return m_Token.Type == type;
}

bool csaw::lang::Parser::At(const std::string& value) const
{
	return m_Token.Value == value;
}

csaw::lang::Token& csaw::lang::Parser::Expect(const TokenType type)
{
	assert(At(type));
	return m_Token;
}

csaw::lang::Token& csaw::lang::Parser::Expect(const std::string& value)
{
	assert(At(value));
	return m_Token;
}

csaw::lang::Token csaw::lang::Parser::GetAndNext()
{
	Token token = m_Token;
	Next();
	return token;
}

csaw::lang::Token csaw::lang::Parser::ExpectAndNext(const TokenType type)
{
	Token token = Expect(type);
	Next();
	return token;
}

csaw::lang::Token csaw::lang::Parser::ExpectAndNext(const std::string& value)
{
	Token token = Expect(value);
	Next();
	return token;
}

bool csaw::lang::Parser::NextIfAt(const TokenType type)
{
	if (At(type))
	{
		Next();
		return true;
	}
	return false;
}

bool csaw::lang::Parser::NextIfAt(const std::string& value)
{
	if (At(value))
	{
		Next();
		return true;
	}
	return false;
}
