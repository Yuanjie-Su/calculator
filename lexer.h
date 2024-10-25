#pragma once
#include<string>
#include<iostream>


namespace Lexer {
	enum class Kind : char {
		name, number, end, print,
		plus = '+', minus = '-', mul = '*', div = '/',
		assign = '=', lp = '(', rp = ')'
	};

	class Token {
	public:
		Kind kind;
		double number_value;
		std::string string_value;
	};

	class Token_stream {
	public:
		Token_stream(std::istream& s) : ip{ &s }, owns{ false } {}
		Token_stream(std::istream* p) : ip{ p }, owns{ true } {}
		~Token_stream() { close(); };

		Token get();
		const Token& current();

		void set_input(std::istream& s) { close(); ip = &s; owns = false; }
		void set_input(std::istream* p) { close(); ip = p; owns = true; }

	private:
		void close() const { if (owns) { delete ip; } };

		std::istream* ip;
		bool owns;
		Token ct {Kind::end};
	};
}