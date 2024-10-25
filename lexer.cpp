#include <cctype>
# include "lexer.h"
# include "error.h"


Lexer::Token Lexer::Token_stream::get() {
	char ch;

	do { //������'\n'����Ŀհ׷�
		if (!ip->get(ch)) return ct = { Kind::end };
	} while (ch != '\n' && isspace(ch));

	switch (ch) {
	case ';':
	case '\n':
		return ct = { Kind::print };
	case '+':
	case '-':
	case '*':
	case '/':
	case '(':
	case ')':
	case '=':
		return ct = { static_cast<Kind>(ch) };
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '.':
		// �ѵ�һ�����֣�����'.'���Żص���������
		ip->putback(ch);
		// �����ֶ���ct
		*ip >> ct.number_value;
		ct.kind = Kind::number;
		return ct;
	default:	// ���֣�����=�����ߴ���
		if (isalpha(ch)) {
			ct.string_value = ch;
			while (ip->get(ch) && isalnum(ch)) {
				// ��ch�ӵ�string_value��ĩβ
				ct.string_value += ch;
			}
			ip->putback(ch);
			ct.kind = Kind::name;
			return ct;
		}
	}
	Error::error("bad token");
	return ct = { Kind::end };
}

const Lexer::Token& Lexer::Token_stream::current() {
	return ct;
}