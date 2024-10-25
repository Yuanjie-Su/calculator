#pragma once
#include<iostream>
#include<istream>


namespace Lexer {
	enum class Kind : char {
		name, number, end,
		plus = '+', minus = '-', mul = '*', div = '/',
		assign = '=', lp = '(', rp = ')', print = ';'
	};

	struct Token {
		Kind kind;
		double number_value;
		std::string string_value;
	};

	class Token_stream {
	public:
		Token_stream(std::istream&);
		Token_stream(std::istream*);
		~Token_stream();

		Token get();
		const Token& current();

		void set_input(std::istream&);
		void set_input(std::istream&);

	private:
		void close();
		std::istream* ip;
		bool owns;
		Token ct;
	};
}

namespace Parser {
	double prim(bool, Lexer::Token_stream&);
	double term(bool, Lexer::Token_stream&);
	double expr(bool, Lexer::Token_stream&);
}

namespace Driver {
	void calculate(Lexer::Token_stream);
}

namespace Error {
	extern int no_of_errors;
	double error(const std::string&);
}

