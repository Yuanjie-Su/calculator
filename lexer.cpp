#include <cctype>
# include "lexer.h"
# include "error.h"


Lexer::Token Lexer::Token_stream::get() {
	char ch;

	do { //跳过除'\n'以外的空白符
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
		// 把第一个数字（或者'.'）放回到输入流中
		ip->putback(ch);
		// 把数字读入ct
		*ip >> ct.number_value;
		ct.kind = Kind::number;
		return ct;
	default:	// 名字，名字=，或者错误
		if (isalpha(ch)) {
			ct.string_value = ch;
			while (ip->get(ch) && isalnum(ch)) {
				// 把ch加到string_value的末尾
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