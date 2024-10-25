#include "driver.h"
#include "parser.h"


void Driver::calculate(Lexer::Token_stream& ts) {
	for (;;) {
		ts.get();
		if (ts.current().kind == Lexer::Kind::end) break;
		if (ts.current().kind == Lexer::Kind::print) continue;
		std::cout << Parser::expr(false, ts) << '\n';
	}
}