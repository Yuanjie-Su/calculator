#include "parser.h"
#include "table.h"

double Parser::prim(bool get, Token_stream& ts) {
	if (get) ts.get();	//读取下一个单词

	double v = 1.0;		// 书中是在case中定义，编译器过不去

	switch (ts.current().kind) {
	case Kind::number: // 浮点数常量
		v = ts.current().number_value;
		ts.get();
		return v;
	case Kind::name:	// 表达式中的“变量”
		// 读取对应的项
		v = Table::table[ts.current().string_value];
		// 如果下一个单词是'=',读取'='后面的表达式的值
		if (ts.get().kind == Kind::assign) {
			v = expr(true, ts);
		}
			
		return v;
	case Kind::minus:	// 一元减法
		return -prim(true, ts);
	case Kind::lp:		// '(表达式'
		// 读取括号内表达式的值
		v = expr(true, ts);
		// 判断'(表达式'下一个单词是否是')'，如果不是则表示')'异常
		if (ts.current().kind != Kind::rp) return error("')' excepted");
		ts.get();		//')'下一个字符
		return v;
	default:
		return error("primary excepted");
	}
}

double Parser::term(bool get, Token_stream& ts) {
	double left = prim(get, ts);

	for (;;) {
		switch (ts.current().kind) {
		case Kind::mul:
			left *= prim(true, ts);
			break;
		case Kind::div:
			if (auto d = prim(true, ts)) {
				left /= d;
				break;
			}
			return error("divide by 0");
		default:
			return left;
		}
	}
}

double Parser::expr(bool get, Token_stream& ts) {
	double left = term(get, ts);

	for (;;) {
		switch (ts.current().kind) {
		case Kind::plus:
			left += term(true, ts);
			break;
		case Kind::minus:
			left -= term(true, ts);
			break;
		default:
			return left;
		}
	}
}